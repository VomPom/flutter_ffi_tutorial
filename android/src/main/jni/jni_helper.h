//
// Created by julis.wang on 2/3/21.
//


#ifndef ANDROID_JNI_HELPER_H
#define ANDROID_JNI_HELPER_H

#endif //ANDROID_JNI_HELPER_H

#include <jni.h>


class JniEnv {
private:
    JNIEnv *env_;
    bool need_detach_;

    JniEnv(const JniEnv &) = delete;

    JniEnv &operator=(const JniEnv &) = delete;

public:
    JniEnv();

    ~JniEnv();

    inline JNIEnv *operator->() {
        return env_;
    }
};

class JniHelper {

public:
    static JavaVM *jvm_;
    static jobject class_loader_;
    static jmethodID find_class_method_;

    static void StoreClassLoader();

    static jclass FindClass(JniEnv &env, const char *name);
//    static void StoreGCMethod();
//    static void TriggerGC(JniEnv &env);
};
