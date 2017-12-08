#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>
#include <caml/threads.h>

#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <png.h>
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


// CAMLprim loadImage(value window, value filename) {
//   CAMLparam2(ocamlWindow, filename);
//   AAssetManager* assetManager = (AssetManager*)(void *)Field(ocamlWindow, 2);

//   AAsset* asset = AAssetManager_open(assetManager, String_val(filename), AASSET_MODE_STREAMING);

//   if (asset == NULL) {
//     CAMLreturn(Val_none);
//   }

//   int png_data_size = AAsset_getLength(asset)
//   void* png_data = AAsset_getBuffer(asset), asset };

//   if (!(png_data != NULL && png_data_size > 8)) {
//     CAMLreturn(Val_none);
//   }
//   if (!png_check_sig((void*)png_data, 8)) {
//     CAMLreturn(Val_none);
//   }

//   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//   // assert(png_ptr != NULL);
//   png_infop info_ptr = png_create_info_struct(png_ptr);
//   // assert(info_ptr != NULL);

//   // ReadDataHandle png_data_handle = (ReadDataHandle) {{png_data, png_data_size}, 0};
//   // png_set_read_fn(png_ptr, &png_data_handle, read_png_data_callback);

//   if (setjmp(png_jmpbuf(png_ptr))) {
//     // LOGI("Error reading PNG file!");
//     CAMLreturn(Val_none);
//   }

//   const PngInfo png_info = read_and_update_info(png_ptr, info_ptr);
//   const DataHandle raw_image = read_entire_png_image(png_ptr, info_ptr, png_info.height);

//   png_read_end(png_ptr, info_ptr);
//   png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

//   return (RawImageData) {
//         png_info.width,
//         png_info.height,
//         raw_image.size,
//         get_gl_color_format(png_info.color_type),
//         raw_image.data};




//   CAMLparam1(filename);
//   CAMLlocal2(record_image_data, dataArr);






//   NSString* name = [NSString stringWithUTF8String:String_val(filename)];
//   CGImageRef spriteImage = [UIImage imageNamed:name].CGImage;
//   if (!spriteImage) {
//     CAMLreturn(Val_none);
//   }

//   size_t width = CGImageGetWidth(spriteImage);
//   size_t height = CGImageGetHeight(spriteImage);

//   uint channels = 4;
//   GLubyte * spriteData = (GLubyte *) calloc(width*height*channels, sizeof(GLubyte));

//   // copy image into the data array
//   CGContextRef spriteContext = CGBitmapContextCreate(
//     spriteData, width, height, 8, width * channels,
//     CGImageGetColorSpace(spriteImage),
//     kCGImageAlphaPremultipliedLast
//   );
//   CGContextDrawImage(spriteContext, CGRectMake(0, 0, width, height), spriteImage);
//   CGContextRelease(spriteContext);





//   record_image_data = caml_alloc_small(4, 0);
//   Field(record_image_data, 0) = Val_int(width);
//   Field(record_image_data, 1) = Val_int(height);
//   Field(record_image_data, 2) = Val_int(channels);

//   // put the data in a bigarray
//   intnat *size = malloc(sizeof(intnat));
//   *size = width * height * channels;
//   dataArr = caml_ba_alloc(CAML_BA_UINT8, 1, spriteData, size);
//   Field(record_image_data, 3) = dataArr;

//   CAMLreturn(Val_some(record_image_data));
// }
