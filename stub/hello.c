#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>
#include <caml/threads.h>
#include "hello.h"

#include <android/log.h>


#include <jni.h>

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "hell-libs::", __VA_ARGS__))







JNIEXPORT jstring JNICALL
Java_com_example_hellolibs_MainActivity_stringFromC(JNIEnv *env, jobject thiz) {

    LOGI("here in c land");

    char* input = "This is hello.c";
    return (*env)->NewStringUTF(env, input);
}












// C stub, notice how we copy values into OCaml and copy it out
char * reasongl_echo(char *buf) {
    static value * func = NULL;
    if (func == NULL) func = caml_named_value("echo");
    caml_callback(*caml_named_value("sayhi"), caml_copy_string("from echo"));
    value v = caml_callback(*func, caml_copy_string(buf));
    return String_val(v);
}

// this is just to make sure we get loaded
// it's a dumb hack.
int dummy () { return 10; }

// Init the runtime
__attribute__ ((__constructor__))
void reasongl_init(void) {
    char *caml_argv[1] = { NULL };
    caml_startup(caml_argv);
    LOGI("Hello from init");
}


// stuff for ocaml to use
void logAndroid(value text) {
  CAMLparam1(text);
  char* str_text = String_val(text);

  __android_log_print(ANDROID_LOG_INFO, "reasongl", str_text);
}

