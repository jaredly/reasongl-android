#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/bigarray.h>

#include <android/log.h>
#include <GLES2/gl2.h>
#import <stdint.h>

#include <jni.h>



void bigarray_unsafe_blit(value arr1, value arr2, value offset, value numOfBytes) {
  char *arr1Data = Caml_ba_data_val(arr1);
  char *arr2Data = Caml_ba_data_val(arr2);
  memcpy(arr2Data + Int_val(offset) * Int_val(numOfBytes), arr1Data, caml_ba_byte_size(Caml_ba_array_val(arr1)));
}




static value oreturn(void *v) {
  CAMLparam0();
  CAMLlocal1(ret);
  ret = caml_alloc_small(1, 0);
  Field(ret, 0) = (long)v;
  return ret;
}

static value unboxed(GLuint i) {
  return Val_int(i);
}

#define Val_none Val_int(0)

static value Val_some(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc_small(1, 0);
  Field(some, 0) = v;
  CAMLreturn(some);
}


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

CAMLprim value getWindowHeight(value ocamlWindow) {
  CAMLparam1(ocamlWindow);
  JNIEnv* env = (JNIEnv*)(void *)Field(ocamlWindow, 0);
  jobject glView = (jobject)(void *)Field(ocamlWindow, 1);

  jclass viewClass = (*env)->GetObjectClass(env, glView);
  jmethodID getHeight = (*env)->GetMethodID(env, viewClass, "getHeight", "()I");
  int height =  (*env)->CallIntMethod(env, glView, getHeight);

  CAMLreturn(Val_int(height));
}

CAMLprim value getWindowWidth(value ocamlWindow) {
  CAMLparam1(ocamlWindow);
  JNIEnv* env = (JNIEnv*)(void *)Field(ocamlWindow, 0);
  jobject glView = (jobject)(void *)Field(ocamlWindow, 1);

  jclass viewClass = (*env)->GetObjectClass(env, glView);
  jmethodID getHeight = (*env)->GetMethodID(env, viewClass, "getWidth", "()I");
  int height =  (*env)->CallIntMethod(env, glView, getHeight);

  CAMLreturn(Val_int(height));
}

