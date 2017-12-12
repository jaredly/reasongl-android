#include <stdio.h>

#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/fail.h>
#include <caml/bigarray.h>

// #include <OpenGL/Gl3.h>
#include <GLES3/gl3.h>

CAMLprim value TglCreateProgram() {
  return (Val_int(glCreateProgram()));
}

CAMLprim value TglCreateShader(value shaderType) {
  return (Val_int(glCreateShader(Int_val(shaderType))));
}

void TglAttachShader(value program, value shader) {
  glAttachShader(Int_val(program), Int_val(shader));
}

void TglDeleteShader(value shader) {
  glDeleteShader(Int_val(shader));
}

void TglShaderSource(value shader, value stringArray) {
  CAMLparam2(shader, stringArray);
  int numOfElements = Wosize_val(stringArray);
  // @MemoryLeak
  const GLchar **arrOfElements = malloc(sizeof(GLchar *) * numOfElements);
  GLint *arrOfLengths = malloc(sizeof(GLint) * numOfElements);
  for(int i = 0; i < numOfElements; ++i) {
    value e = Field(stringArray, i);
    arrOfElements[i] = String_val(e);
    arrOfLengths[i] = caml_string_length(e);
  }
  glShaderSource(Int_val(shader), numOfElements, arrOfElements, arrOfLengths);
  CAMLreturn0;
}

void TglCompileShader(value shader) {
  glCompileShader(Int_val(shader));
}

void TglLinkProgram(value program) {
  glLinkProgram(Int_val(program));
}

void TglUseProgram(value program) {
  glUseProgram(Int_val(program));
}

CAMLprim value TglGenBuffers(value count) {
  CAMLparam1(count);
  CAMLlocal1(ret);

  int size = Int_val(count);
  // @Speed
  unsigned int *buffers = malloc(sizeof(unsigned int) * size);
  glGenBuffers(size, buffers);

  ret = caml_alloc_small(size, 0);
  for (int i = 0; i < size; ++i) {
    Field(ret, i) = Val_int(buffers[i]);
  }
  CAMLreturn(ret);
}

value TglGenBuffer() {
  unsigned int buffers = 0;
  glGenBuffers(1, &buffers);
  return (Val_int(buffers));
}

void TglClearColor(value r, value g, value b, value a) {
  glClearColor(Double_val(r), Double_val(g), Double_val(b), Double_val(a));
}

void TglBindBuffer(value kind, value buffer) {
  glBindBuffer(Int_val(kind), Int_val(buffer));
}

CAMLprim value TglGenTextures(value count) {
  CAMLparam1(count);
  CAMLlocal1(ret);

  int size = Int_val(count);
  unsigned int textures[size];
  glGenTextures(size, textures);

  ret = caml_alloc_small(size, 0);
  for (int i = 0; i < size; ++i) {
    Field(ret, i) = Val_int(textures[i]);
  }

  CAMLreturn(ret);
}

value TglGenTexture() {
  unsigned int textures = 0;
  glGenTextures(1, &textures);
  return (Val_int(textures));
}

void TglActiveTexture(value textureUnit) {
  glActiveTexture(Int_val(textureUnit));
}

void TglBindTexture(value kind, value texture) {
  glBindTexture(Int_val(kind), Int_val(texture));
}

void TglTexSubImage2D_native(value target, value level, value xoffset, value yoffset, value width, value height, value format, value type, value pixels) {
  CAMLparam5(target, level, xoffset, yoffset, width);
  CAMLxparam4(height, format, type, pixels);

  glTexSubImage2D(Int_val(target), Int_val(level), Int_val(xoffset), Int_val(yoffset), Int_val(width), Int_val(height), Int_val(format), Int_val(type), Caml_ba_data_val(pixels));
  CAMLreturn0;
}

void TglTexSubImage2D_bytecode(value * argv, int argn) {
  TglTexSubImage2D_native(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
}


void TglTexParameteri(value kind, value pname, value param) {
  glTexParameteri(Int_val(kind), Int_val(pname), Int_val(param));
}

void TglEnable(value thing) {
  glEnable(Int_val(thing));
}

void TglDisable(value thing) {
  glDisable(Int_val(thing));
}

void TglBlendFunc(value sfactor, value dfactor) {
  glBlendFunc(Int_val(sfactor), Int_val(dfactor));
}

CAMLprim value TglReadPixels_RGBA(value x, value y, value width, value height) {
  CAMLparam4(x, y, width, height);
  CAMLlocal1(ret);

  // Allocate a pointer for caml_ba_alloc's sake.
  intnat *size = malloc(sizeof(intnat));
  *size = Int_val(width) * Int_val(height) * 4;

  char *data = malloc(*size * sizeof(char));

  glReadPixels(Int_val(x), Int_val(y), Int_val(width), Int_val(height), GL_RGBA, GL_UNSIGNED_BYTE, data);

  // return array of size `size` of dimension 1 of uint8 (char).
  ret = caml_ba_alloc(CAML_BA_UINT8, 1, data, size);
  CAMLreturn(ret);
}

void TglTexImage2D_RGBA_native(value target, value level, value width, value height, value border, value data) {
  glTexImage2D(Int_val(target), Int_val(level), 4, Int_val(width), Int_val(height), Int_val(border), GL_RGBA, GL_UNSIGNED_BYTE, Caml_ba_data_val(data));
  // char datax[4] = {0, 0, 0, 0};
  // glTexImage2D(Int_val(target), Int_val(level), 4, 1, 1, 0, GL_ARGB, GL_UNSIGNED_BYTE,
  //   // Caml_ba_data_val(data)
  //   datax
  // );
}

void TglTexImage2D_RGBA_bytecode(value * argv, int argn) {
  TglTexImage2D_RGBA_native(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
}

void TglUniform1i(value location, value v) {
  glUniform1i(Int_val(location), Int_val(v));
}

void TglUniform1f(value location, value v) {
  glUniform1f(Int_val(location), Double_val(v));
}

void TglUniform2f(value location, value v1, value v2) {
  glUniform2f(Int_val(location), Double_val(v1), Double_val(v2));
}

void TglUniform3f(value location, value v1, value v2, value v3) {
  glUniform3f(Int_val(location), Double_val(v1), Double_val(v2), Double_val(v3));
}

void TglUniform4f(value location, value v1, value v2, value v3, value v4) {
  glUniform4f(Int_val(location), Double_val(v1), Double_val(v2), Double_val(v3), Double_val(v4));
}


void TglBufferData(value target, value data, value usage) {
  glBufferData(Int_val(target), caml_ba_byte_size(Caml_ba_array_val(data)), Caml_ba_data_val(data), Int_val(usage));
}

void TglViewport(value x, value y, value width, value height) {
  glViewport(Int_val(x), Int_val(y), Int_val(width), Int_val(height));
}

void TglClear(value mask) {
  glClear(Int_val(mask));
}

value TglGetUniformLocation(value program, value name) {
  return (Val_int(glGetUniformLocation(Int_val(program), String_val(name))));
}

value TglGetAttribLocation(value program, value name) {
  return (Val_int(glGetAttribLocation(Int_val(program), String_val(name))));
}

void TglEnableVertexAttribArray(value attrib) {
  glEnableVertexAttribArray(Int_val(attrib));
}

void TglVertexAttribPointer_native(value index, value size, value typ, value normalize, value stride, value offset) {
  CAMLparam5(index, size, typ, normalize, stride);
  CAMLxparam1(offset);
  long o = (long)Int_val(offset);
  glVertexAttribPointer(Int_val(index), Int_val(size), Int_val(typ), Bool_val(normalize), Int_val(stride), (const GLvoid *)o);
  CAMLreturn0;
}

void TglVertexAttribPointer_bytecode(value * argv, int argn) {
  TglVertexAttribPointer_native(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
}

void TglVertexAttribDivisor(intnat attrib, intnat divisor) {
  glVertexAttribDivisor(attrib, divisor);
}

void TglVertexAttribDivisor_byte(value attrib, value divisor) {
  glVertexAttribDivisor(Int_val(attrib), Int_val(divisor));
}

value TglGetProgramiv(value program, value pname) {
  GLint ret;
  glGetProgramiv(Int_val(program), Int_val(pname), &ret);
  return (Val_int(ret));
}

value TglGetShaderiv(value shader, value pname) {
  GLint ret;
  glGetShaderiv(Int_val(shader), Int_val(pname), &ret);
  return (Val_int(ret));
}

CAMLprim value TglGetShaderInfoLog(value shader) {
  CAMLparam1(shader);

  GLint exactLength;
  glGetShaderiv(Int_val(shader), GL_INFO_LOG_LENGTH, &exactLength);
  GLchar buffer[exactLength];
  glGetShaderInfoLog(Int_val(shader), exactLength - 1, NULL, buffer);

  CAMLreturn(caml_copy_string(buffer));
}

CAMLprim value TglGetProgramInfoLog(value program) {
  CAMLparam1(program);

  GLint exactLength;
  glGetProgramiv(Int_val(program), GL_INFO_LOG_LENGTH, &exactLength);

  GLchar buffer[exactLength];
  glGetProgramInfoLog(Int_val(program), exactLength - 1, NULL, buffer);

  CAMLreturn(caml_copy_string(buffer));
}

CAMLprim value TglGetShaderSource(value shader) {
  CAMLparam1(shader);

  GLint exactLength;
  glGetShaderiv(Int_val(shader), GL_SHADER_SOURCE_LENGTH, &exactLength);

  GLchar buffer[exactLength];
  glGetShaderSource(Int_val(shader), exactLength - 1, NULL, buffer);

  CAMLreturn(caml_copy_string(buffer));
}

void TglDrawArrays(value mode, value first, value count) {
  glDrawArrays(Int_val(mode), Int_val(first), Int_val(count));
}

void TglDrawElements(value mode, value first, value typ, value offset) {
  long o = (long)Int_val(offset);
  glDrawElements(Int_val(mode), Int_val(first), Int_val(typ), (const GLvoid *)o);
}

void TglDrawElementsInstanced(value mode, value first, value typ, value indices, value primcount) {
  long o = (long)Int_val(indices);
  glDrawElementsInstanced(Int_val(mode), Int_val(first), Int_val(typ), (const GLvoid *)o, Int_val(primcount));
}

void TglUniformMatrix4fv(value location, value transpose, value val) {
  float matrix[16];
  for (int i = 0; i < 16; ++i){
    matrix[i] = Double_field(val, i);
  }
  glUniformMatrix4fv(Int_val(location), 1, Bool_val(transpose), matrix);
}