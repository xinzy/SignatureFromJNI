#include <jni.h>
#include <stdlib.h>
#include <malloc.h>
#include <android/log.h>

#define JNI_FN(A) Java_com_xinzy_signature_util_Fingerprint_ ## A

#define DEBUG 1

#ifdef DEBUG

#define LOG_TAG "Fingerprint"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#else

#define LOGI(...) /////////
#define LOGE(...) /////////

#endif

#ifndef _Included_com_xinzy_signature_util_Fingerprint
#define _Included_com_xinzy_signature_util_Fingerprint
#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 */
JNIEXPORT jstring JNICALL JNI_FN(getSign) (JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
#endif

