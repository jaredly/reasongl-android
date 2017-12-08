
SYSROOT=~/.opam/4.04.0-android32/android-sysroot

opam switch 4.04.0-android32
eval `opam config env`

set -ex

rm -rf build
mkdir -p build/src build/src

cp src/* build/src
# cp src/* build/src

OCAMLOPT="$SYSROOT/bin/ocamlopt -ccopt -fno-omit-frame-pointer -ccopt -O3 -ccopt -fPIC -I build/src -I build/src -I $SYSROOT/lib/ocaml -runtime-variant _pic"

REFMT=~/.opam/4.04.2/bin/refmt

# Compile to a .o
$OCAMLOPT -c -o build/src/Capi.cmx -pp "$REFMT --print binary" -impl build/src/Capi.re
# $OCAMLOPT -c -o build/src/App.cmx -pp "$REFMT --print binary" -impl build/src/App.re
$OCAMLOPT -c -o build/src/MLforJava.cmx -pp "$REFMT --print binary" -impl build/src/MLforJava.re

$OCAMLOPT -ccopt -std=c11 -c build/src/CforJava.c
mv CforJava.o build/src
$OCAMLOPT -ccopt -std=c11 -c build/src/CforOCaml.c
mv CforOCaml.o build/src

$OCAMLOPT -output-obj -ccopt -fPIC -ccopt -pie -ccopt -llog -ccopt -landroid \
  -o libfrom_ocaml.so \
  build/src/Capi.cmx \
  build/src/CforOCaml.o build/src/CforJava.o \
  build/src/MLforJava.cmx \
  $SYSROOT/lib/ocaml/libasmrun.a

# Move into our android project
DEST=example-project/app/src/main/jniLibs/armeabi-v7a
mkdir -p $DEST
mv libfrom_ocaml.so $DEST/libreasongl.so

# Java_com_example_hellolibs_OCamlBindings_reasonglMain