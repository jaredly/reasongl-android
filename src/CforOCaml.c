#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>
#include <caml/threads.h>
#include <caml/bigarray.h>

#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <png.h>
// #include <GLES3/gl3.h>
#include <GLES2/gl2.h>

#include <jni.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "reasongl",__VA_ARGS__)

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
  // CAMLreturn(Val_int(1000));
}

CAMLprim value getWindowWidth(value ocamlWindow) {
  CAMLparam1(ocamlWindow);
  JNIEnv* env = (JNIEnv*)(void *)Field(ocamlWindow, 0);
  jobject glView = (jobject)(void *)Field(ocamlWindow, 1);

  jclass viewClass = (*env)->GetObjectClass(env, glView);
  jmethodID getHeight = (*env)->GetMethodID(env, viewClass, "getWidth", "()I");
  int height =  (*env)->CallIntMethod(env, glView, getHeight);

  CAMLreturn(Val_int(height));
  // CAMLreturn(Val_int(500));
}

#define Val_none Val_int(0)

static value Val_some(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc_small(1, 0);
  Field(some, 0) = v;
  CAMLreturn(some);
}

void texImage2DWithBitmap(value ocamlWindow, value target, value level, value bitmap, value border) {
  CAMLparam5(ocamlWindow, target, level, bitmap, border);

  JNIEnv* g_env = (JNIEnv*)(void *)Field(ocamlWindow, 0);
  jobject g_pngmgr = (jobject)(void *)Field(ocamlWindow, 2);

  jclass cls = (*g_env)->GetObjectClass(g_env, g_pngmgr);

  jmethodID method = (*g_env)->GetMethodID(g_env, cls, "texImage2DWithBitmap", "(IILandroid/graphics/Bitmap;I)V");
  (*g_env)->CallVoidMethod(g_env, g_pngmgr, method, Int_val(target), Int_val(level), (jobject)(void*)bitmap, Int_val(border));
}

CAMLprim value loadImage(value ocamlWindow, value filename) {
  CAMLparam2(ocamlWindow, filename);
  CAMLlocal1(record_image_data);

  JNIEnv* g_env = (JNIEnv*)(void *)Field(ocamlWindow, 0);
  jobject g_pngmgr = (jobject)(void *)Field(ocamlWindow, 2);

  jclass cls = (*g_env)->GetObjectClass(g_env, g_pngmgr);

  /* Ask the PNG manager for a bitmap */
  jmethodID openBitmap = (*g_env)->GetMethodID(g_env, cls, "openBitmap",
                          "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
  jstring name = (*g_env)->NewStringUTF(g_env, String_val(filename));
  jobject png = (*g_env)->CallObjectMethod(g_env, g_pngmgr, openBitmap, name);
  (*g_env)->DeleteLocalRef(g_env, name);

  if (!png) {
    CAMLreturn(Val_none);
  } else {
    /* Get image dimensions */
    jmethodID getBitmapWidth = (*g_env)->GetMethodID(g_env, cls, "getBitmapWidth", "(Landroid/graphics/Bitmap;)I");
    int width = (*g_env)->CallIntMethod(g_env, g_pngmgr, getBitmapWidth, png);
    jmethodID getBitmapHeight = (*g_env)->GetMethodID(g_env, cls, "getBitmapHeight", "(Landroid/graphics/Bitmap;)I");
    int height = (*g_env)->CallIntMethod(g_env, g_pngmgr, getBitmapHeight, png);

    record_image_data = caml_alloc_small(3, 0);
    Field(record_image_data, 0) = Val_int(width);
    Field(record_image_data, 1) = Val_int(height);
    Field(record_image_data, 2) = (long)png;
    CAMLreturn(Val_some(record_image_data));
  }
}

CAMLprim value loadFile(value ocamlWindow, value filename) {
  CAMLparam2(ocamlWindow, filename);

  JNIEnv* g_env = (JNIEnv*)(void *)Field(ocamlWindow, 0);
  jobject g_pngmgr = (jobject)(void *)Field(ocamlWindow, 2);

  jclass cls = (*g_env)->GetObjectClass(g_env, g_pngmgr);

  jmethodID method = (*g_env)->GetMethodID(g_env, cls, "readFileContents",
                          "(Ljava/lang/String;)Ljava/lang/String;");
  jstring name = (*g_env)->NewStringUTF(g_env, String_val(filename));
  jstring contents = (*g_env)->CallObjectMethod(g_env, g_pngmgr, method, name);
  (*g_env)->DeleteLocalRef(g_env, name);

  if (contents) {
    CAMLreturn(Val_some(caml_copy_string(contents)));
  } else {
    CAMLreturn(Val_none);
  }
}

void saveString(value ocamlWindow, value key, value data) {
  CAMLparam3(ocamlWindow, key, data);

  JNIEnv* g_env = (JNIEnv*)(void *)Field(ocamlWindow, 0);
  jobject g_pngmgr = (jobject)(void *)Field(ocamlWindow, 2);

  jclass cls = (*g_env)->GetObjectClass(g_env, g_pngmgr);

  jmethodID method = (*g_env)->GetMethodID(g_env, cls, "saveUserData",
                          "(Ljava/lang/String;Ljava/lang/String;)V");
  jstring name = (*g_env)->NewStringUTF(g_env, String_val(key));
  jstring contents = (*g_env)->NewStringUTF(g_env, String_val(data));
  (*g_env)->CallObjectMethod(g_env, g_pngmgr, method, name, contents);
  (*g_env)->DeleteLocalRef(g_env, name);
  (*g_env)->DeleteLocalRef(g_env, contents);

}

CAMLprim value loadString(value ocamlWindow, value key) {
  CAMLparam2(ocamlWindow, key);

  JNIEnv* g_env = (JNIEnv*)(void *)Field(ocamlWindow, 0);
  jobject g_pngmgr = (jobject)(void *)Field(ocamlWindow, 2);

  jclass cls = (*g_env)->GetObjectClass(g_env, g_pngmgr);

  jmethodID method = (*g_env)->GetMethodID(g_env, cls, "loadUserData",
                          "(Ljava/lang/String;)Ljava/lang/String;");
  jstring name = (*g_env)->NewStringUTF(g_env, String_val(key));
  jstring contents = (*g_env)->CallObjectMethod(g_env, g_pngmgr, method, name);
  (*g_env)->DeleteLocalRef(g_env, name);

  CAMLreturn(caml_copy_string(contents));
}