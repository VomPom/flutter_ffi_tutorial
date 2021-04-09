//
// Created by julis.wang on 2/3/21.
//


#include "jni_helper.h"


JavaVM *JniHelper::jvm_;
jobject JniHelper::class_loader_;
jmethodID JniHelper::find_class_method_;


JniEnv::JniEnv() : env_{nullptr}, need_detach_{false} {
    if (JniHelper::jvm_->GetEnv((void **) &env_, JNI_VERSION_1_6) == JNI_EDETACHED) {
        JniHelper::jvm_->AttachCurrentThread(&env_, nullptr);
        need_detach_ = true;
    }
}

JniEnv::~JniEnv() {
    if (need_detach_) JniHelper::jvm_->DetachCurrentThread();
}

void JniHelper::StoreClassLoader() {
    JniEnv env;
    auto random_class = env->FindClass("com/example/flutter_ffi_tutorial/FlutterFfiTutorialPlugin");
    auto class_class = env->GetObjectClass(random_class);
    auto class_loader_class = env->FindClass("java/lang/ClassLoader");
    auto get_class_loader_method = env->GetMethodID(class_class, "getClassLoader",
                                                    "()Ljava/lang/ClassLoader;");
    class_loader_ = env->NewGlobalRef(env->CallObjectMethod(random_class, get_class_loader_method));
    find_class_method_ = env->GetMethodID(class_loader_class, "findClass",
                                          "(Ljava/lang/String;)Ljava/lang/Class;");
}

jclass JniHelper::FindClass(JniEnv &env, const char *name) {
    return reinterpret_cast<jclass>(env->CallObjectMethod(JniHelper::class_loader_,
                                                          JniHelper::find_class_method_,
                                                          env->NewStringUTF(name)));
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    // Cache the JavaVM.
    JniHelper::jvm_ = vm;
    JniHelper::StoreClassLoader();
    return JNI_VERSION_1_6;
}
