#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>
#include <caml/threads.h>

#include <android/log.h>

#include <jni.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "reasongl",__VA_ARGS__)

#define JNI_METHOD(returnType, name) JNIEXPORT returnType JNICALL Java_com_example_hellolibs_OCamlBindings_ ## name

#define CALL_OCAML_FN(name, arg) static value *ocaml_fn = NULL; \
  if (ocaml_fn == NULL) ocaml_fn = caml_named_value(name); \
  if (ocaml_fn == NULL) LOGI("Function not assigned %s", name); \
  else caml_callback(*ocaml_fn, arg);

#define CALL_OCAML_FN2(name, arg1, arg2) static value *ocaml_fn = NULL; \
  if (ocaml_fn == NULL) ocaml_fn = caml_named_value(name); \
  if (ocaml_fn == NULL) LOGI("Function not assigned %s", name); \
  else caml_callback2(*ocaml_fn, arg1, arg2);


/************ OCaml <-> C <-> Java ***************/

// passing objects around https://stackoverflow.com/questions/2504334/passing-data-types-from-java-to-c-or-vice-versa-using-jni

JNI_METHOD(void, reasonglMain)(JNIEnv* env, jobject obj, jobject glView) {
  CAMLparam0();
  CAMLlocal1(ocamlWindow);
  ocamlWindow = caml_alloc_small(2, 0);
  Field(ocamlWindow, 0) = (long)env;
  Field(ocamlWindow, 1) = (long)glView;
  // ocamlWindow = caml_copy_double(1.0);

  /*
  jclass viewClass = (*env)->GetObjectClass(env, glView);
  jmethodID getHeight = (*env)->GetMethodID(env, viewClass, "getHeight", "()I");
  int height =  (*env)->CallIntMethod(env, glView, getHeight);

  __android_log_print(ANDROID_LOG_INFO, "reasongl", "OCAML Hight %d", height);
  */

  LOGI("Before main");
  CALL_OCAML_FN("reasonglMain", ocamlWindow);
  LOGI("After main");
  CAMLreturn0;
}

JNI_METHOD(void, reasonglUpdate)(JNIEnv* env, jobject obj, jdouble timeSinceLastDraw) {
  CALL_OCAML_FN("reasonglUpdate", caml_copy_double(timeSinceLastDraw));
}

JNI_METHOD(void, reasonglTouchPress)(JNIEnv* env, jobject obj, jdouble x, jdouble y) {
  CALL_OCAML_FN2("reasonglTouchPress", caml_copy_double(x), caml_copy_double(y));
}

JNI_METHOD(void, reasonglTouchDrag)(JNIEnv* env, jobject obj, jdouble x, jdouble y) {
  CALL_OCAML_FN2("reasonglTouchDrag", caml_copy_double(x), caml_copy_double(y));
}

JNI_METHOD(void, reasonglTouchRelease)(JNIEnv* env, jobject obj, jdouble x, jdouble y) {
  CALL_OCAML_FN2("reasonglTouchRelease", caml_copy_double(x), caml_copy_double(y));
}

