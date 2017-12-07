
DEST=example-project/app/src/main/jniLibs/armeabi-v7a

SYSROOT=~/.opam/4.04.0-android32/android-sysroot
NDK_BIN=~/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/arm-linux-androideabi/bin

opam switch 4.04.0-android32
eval `opam config env`

set -ex

rm -rf build
mkdir -p build/src build/stub

cp src/* build/src
cp stub/* build/stub

OCAMLOPT="$SYSROOT/bin/ocamlopt -ccopt -fno-omit-frame-pointer -ccopt -O3 -ccopt -fPIC -I build/src -I build/stub -I $SYSROOT/lib/ocaml -runtime-variant _pic"

# Compile to a .o
$OCAMLOPT -c -o build/stub/Capi.cmx build/stub/Capi.ml
$OCAMLOPT -c -o build/src/App.cmx build/src/App.ml
$OCAMLOPT -c -o build/stub/MLforJava.cmx build/stub/MLforJava.ml

$OCAMLOPT -ccopt -std=c11 -c build/stub/CforJava.c
mv CforJava.o build/stub
$OCAMLOPT -ccopt -std=c11 -c build/stub/CforOCaml.c
mv CforOCaml.o build/stub

$OCAMLOPT -output-obj -ccopt -llog -ccopt -landroid \
  -o libfrom_ocaml.so \
  build/stub/Capi.cmx \
  build/src/App.cmx \
  build/stub/CforOCaml.o build/stub/CforJava.o \
  build/stub/MLforJava.cmx \
  $SYSROOT/lib/ocaml/libasmrun.a

# Move into our android project
mkdir -p $DEST
cp libfrom_ocaml.so $DEST/libreasongl.so

