#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>
#include <caml/threads.h>

#include <android/log.h>
// #include <GLES3/gl3.h>
#include <GLES2/gl2.h>

#include <jni.h>

// Init the runtime
__attribute__ ((__constructor__))
void reasongl_init(void) {
    char *caml_argv[1] = { NULL };
    caml_startup(caml_argv);
}

void logAndroid(value text) {
  CAMLparam1(text);
  char* str_text = String_val(text);

  __android_log_print(ANDROID_LOG_INFO, "reasongl", str_text);
}

