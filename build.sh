
DEST=example-project/distribution/reasongl/lib/armeabi-v7a/
HDEST=example-project/distribution/reasongl/include
# TODO move these into this repo, not depend on externals
SYSROOT=~/.opam/4.04.0-android32/android-sysroot
NDK_BIN=~/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/arm-linux-androideabi/bin

opam switch 4.04.0-android32
eval `opam config env`

set -ex

mkdir -p build/src build/stub

cp src/* build/src
cp stub/* build/stub

OCAMLOPT="$SYSROOT/bin/ocamlopt -ccopt -fno-omit-frame-pointer -ccopt -O3 -ccopt -fPIC -I build/src -I build/stub -I $SYSROOT/lib/ocaml -runtime-variant _pic"

# Compile to a .o
$OCAMLOPT -c -o build/src/Hello.cmx build/src/Hello.ml
$OCAMLOPT -c -o build/stub/bindings.cmx build/stub/bindings.ml
$OCAMLOPT -ccopt -std=c11 -c build/stub/hello.c
mv hello.o build/stub/hello.o

$OCAMLOPT -output-obj -o libhelloworld.o build/src/Hello.cmx build/stub/hello.o build/stub/bindings.cmx

# Connect with the runtime
cp $SYSROOT/lib/ocaml/libasmrun.a libreasongl.a
$NDK_BIN/ar -r libreasongl.a libhelloworld.o build/stub/hello.o
$NDK_BIN/ranlib libreasongl.a

# Move into our android project
mkdir -p $DEST
cp libreasongl.a $DEST
cp stub/hello.h $HDEST/reasongl.h

