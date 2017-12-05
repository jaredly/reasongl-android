//
// Created by Jared on 12/4/17.
//

#include <string.h>
#include <jni.h>

 // extern void caml_startup(char** args);

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   hello-jni/app/src/main/java/com/example/hellojni/HelloJni.java
 */
JNIEXPORT jstring JNICALL
Java_com_jaredforsyth_reasonglexample_MainActivity_stringFromJNI2( JNIEnv* env, jobject thiz )
{
     // char* args[] = {NULL};
     // caml_startup(args);
    return (*env)->NewStringUTF(env, "Hello from JNI! 2.");
}