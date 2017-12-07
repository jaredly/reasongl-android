// TODO get rid of this file, and just have an ocaml-created shared lib
#include <cstring>
#include <jni.h>
#include <cinttypes>
#include <reasongl.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellolibs_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    char* input = "hello from the jni right here";
    return env->NewStringUTF(reasongl_echo(input));
}
