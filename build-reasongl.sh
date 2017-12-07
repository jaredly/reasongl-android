set -ex

DEST=example-project/distribution/reasongl/lib/armeabi-v7a/
# TODO move these into this repo, not depend on externals
SYSROOT=~/.opam/4.04.0-android32/android-sysroot
NDK_BIN=~/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/arm-linux-androideabi/bin

opam switch 4.04.0-android32
eval `opam config env`

# Compile to a .o
$SYSROOT/bin/ocamlopt \
  -ccopt -fno-omit-frame-pointer \
  -ccopt -O3 \
  -ccopt -fPIC \
  -runtime-variant _pic \
  -output-obj -linkall \
  -o libhelloworld.o src/Hello.ml

# Connect with the runtime
cp $SYSROOT/lib/ocaml/libasmrun.a libreasongl.a
$NDK_BIN/ar -r libreasongl.a libhelloworld.o
$NDK_BIN/ranlib libreasongl.a

# Move into our android project
mkdir -p $DEST
mv libreasongl.a $DEST

