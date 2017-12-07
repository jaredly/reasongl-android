#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>
#include <caml/threads.h>

#include <android/log.h>

#include <jni.h>

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "hello-libs::", __VA_ARGS__))

#define JNI_METHOD(returnType, name) JNIEXPORT returnType JNICALL Java_com_example_hellolibs_OCamlBindings_ ## name

/************ C <-> OCaml ***************
char * reasongl_echo(char *buf) {
    static value * func = NULL;
    if (func == NULL) func = caml_named_value("echo");
    caml_callback(*caml_named_value("sayhi"), caml_copy_string("from echo"));
    value v = caml_callback(*func, caml_copy_string(buf));
    return String_val(v);
}

************ C <-> Java ***************
JNI_METHOD(jstring, stringFromC)(JNIEnv *env, jobject thiz) {
    return (*env)->NewStringUTF(env, reasongl_echo("This is CforJava.c"));
}
*/


/************ OCaml <-> C <-> Java ***************/
JNI_METHOD(jstring, stringFromC)(JNIEnv *env, jobject thiz) {
    static value * func = NULL;
    if (func == NULL) func = caml_named_value("echo");

    char *buf = "This is CforJava.c";
    value v = caml_callback(*func, caml_copy_string(buf));

    return (*env)->NewStringUTF(env, String_val(v));
}



