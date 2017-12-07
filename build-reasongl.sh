set -ex
SYSROOT=android-sysroot-armv7
DEST=example-project/distribution/reasongl/lib/armeabi-v7a/

opam switch 4.04.0-android32
eval `opam config env`

ocamlfind -toolchain android ocamlopt -linkpkg -ccopt -fPIC  -ccopt -fno-omit-frame-pointer -ccopt -fpic -output-obj -linkall -runtime-variant _pic old_not_working/helloworld.ml $SYSROOT/lib/ocaml/stdlib.a -o libhelloworld.o
cp $SYSROOT/lib/ocaml/libasmrun.a libreasongl.a
ar -r libreasongl.a libhelloworld.o
~/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-ranlib libreasongl.a
mkdir -p $DEST
mv libreasongl.a $DEST

