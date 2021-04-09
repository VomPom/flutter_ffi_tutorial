#include <jni.h>
#include <android/log.h>
#include <map>
#include <mutex>

#include "jni_helper.h"
#include "ffi_async_utils.h"

#define FFI_EXPORT extern "C" __attribute__((visibility("default"))) __attribute__((used))

using namespace std;

static jclass j_handle_dart_class = nullptr;
static jclass j_handle_dart_sync_class = nullptr;

static jmethodID j_handle_dart_method = nullptr;
static jmethodID j_handle_dart_sync_method = nullptr;

bool useJava = true;

const char *handle_dart_class_name = useJava
                                     ? "com/example/flutter_ffi_tutorial/HandleFFiMessageManagerJava"
                                     : "com/example/flutter_ffi_tutorial/HandleFFiMessageManager";

typedef void(*call_back_fun)(const char *, int);

static std::map<std::string, call_back_fun> call_back_map;
static std::mutex call_back_map_mutex;


FFI_EXPORT void c_with_out_return_value(int value) {
    LOG_D("Got invoke value: %d", value);
}

FFI_EXPORT int c_with_return_value(int value1, int value2) {
    int result = value1 + value2;
    LOG_D("Got invoke value1: %d, value2:%d result:%d", value1, value2, result);
    return result;
}

FFI_EXPORT int platform_sync(int value1, int value2) {
    static JniEnv sync_env;

    if (j_handle_dart_sync_class == nullptr || j_handle_dart_sync_method == nullptr) {
        j_handle_dart_sync_class = (jclass) sync_env->NewGlobalRef(
                JniHelper::FindClass(sync_env, handle_dart_class_name));
        j_handle_dart_sync_method = sync_env->GetStaticMethodID(j_handle_dart_sync_class,
                                                                "handleFFISyncCall", "(II)I");
    }

    auto result = sync_env->CallStaticIntMethod(j_handle_dart_sync_class,
                                                j_handle_dart_sync_method,
                                                value1, value2);
    return result;
}



FFI_EXPORT void
platform_async(char *c_pt_id, Dart_Port send_port, int value1, int value2,
               void (*callback)(const char *, int)) {
    //Single flutter Engine leads to only one thread will call this method, cache JniEnv to save time.
    static JniEnv async_env;
    if (j_handle_dart_class == nullptr || j_handle_dart_method == nullptr) {
        j_handle_dart_class = (jclass) async_env->NewGlobalRef(
                JniHelper::FindClass(async_env, handle_dart_class_name));
        j_handle_dart_method = async_env->GetStaticMethodID(j_handle_dart_class,
                                                            "handleFFIAsyncCall",
                                                            "(IIJLjava/lang/String;)V");
    }
    jstring cid_str = async_env->NewStringUTF(c_pt_id);

    async_env->CallStaticVoidMethod(j_handle_dart_class, j_handle_dart_method, value1, value2,
                                    send_port, cid_str);

    std::string str_id = c_pt_id;
    call_back_map_mutex.lock();
    call_back_map.insert(pair<std::string, call_back_fun>(str_id, callback));
    call_back_map_mutex.unlock();
    async_env->DeleteLocalRef(cid_str);
}


char *jByteArrayToChar(JNIEnv *env, jbyteArray buf) {
    char *chars = NULL;
    jbyte *bytes;
    bytes = env->GetByteArrayElements(buf, 0);
    int chars_len = env->GetArrayLength(buf);
    chars = new char[chars_len + 1];
    memset(chars, 0, chars_len + 1);
    memcpy(chars, bytes, chars_len);
    chars[chars_len] = 0;
    env->ReleaseByteArrayElements(buf, bytes, 0);
    return chars;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_flutter_1ffi_1tutorial_HandleFFiMessageManager_sendMessageToNative(JNIEnv *env,
                                                                                    jobject thiz,
                                                                                    jint result,
                                                                                    jlong send_port,
                                                                                    jbyteArray cid) {

    std::string str_cid = jByteArrayToChar(env, cid);
    const char *cid_char = str_cid.data();
    call_back_fun _callback;
    call_back_map_mutex.lock();
    auto iterator = call_back_map.find(cid_char);
    if (iterator == call_back_map.end()) {
        call_back_map_mutex.unlock();
        return;
    }
    _callback = iterator->second;
    call_back_map.erase(iterator);
    call_back_map_mutex.unlock();

    Work work = [_callback, cid_char, result] {
        if (_callback != nullptr) {
            _callback(cid_char, result);

        } else {
            LOG_E("_callback == null");
        }
    };

    // Copy to heap to make it outlive the function scope.
    const Work *work_ptr = new Work(work);
    NotifyDart(send_port, work_ptr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_flutter_1ffi_1tutorial_HandleFFiMessageManagerJava_sendMessageToNative(JNIEnv *env,
                                                                                        jclass clazz,
                                                                                        jint result,
                                                                                        jlong dart_send_port,
                                                                                        jbyteArray cid) {

    Java_com_example_flutter_1ffi_1tutorial_HandleFFiMessageManager_sendMessageToNative(env, clazz,
                                                                                        result,
                                                                                        dart_send_port,
                                                                                        cid);
}