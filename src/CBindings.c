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


CAMLprim value getTimeMs() {
  CAMLparam0();
  CAMLreturn(caml_copy_double(gettimeofday()));
}



// CAMLprim value loadImage(value filename) {
//   CAMLparam1(filename);
//   CAMLreturn(Val_none);
// }