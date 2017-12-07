
# ocamlfind -toolchain android ocamlopt -output-complete-obj helloworld.ml -o libreasongl.so
# ocamlfind -toolchain android ocamlopt -output-obj -cclib -shared helloworld.ml -o libreasongl.so
set -ex

if [ -z "old" ]; then
  echo "static compile maybe"
  ocamlfind -toolchain android ocamlopt -linkpkg -output-complete-obj helloworld.ml libasmrun.a -o libhelloworld.o
  cp libasmrun.a libreasongl.a
  # ar -r libreasongl.a libhelloworld.o
  # ranlib libreasongl.a
  ~/.opam/4.04.0+32bit/android-ndk/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-ranlib libreasongl.a
  cp libreasongl.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
fi


# or shared?
if [ -z "wont build" ]; then
  echo "shared compile"
  ocamlfind -toolchain android ocamlopt -package re.pcre -linkpkg -output-obj -cclib -shared helloworld.ml libasmrun.a -o libreasongl.so
  cp libasmrun_shared.so android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
  cp libreasongl.so android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
fi

# http://www.camlcity.org/knowledge/kb_002_shared_library.html

if [ -z "segfaulted" ]; then
  echo "static compile maybe"
  ocamlfind -toolchain android ocamlopt -linkpkg -output-obj helloworld.ml libasmrun.a -o libhelloworld.o
  cp libasmrun.a libreasongl.a
  ar -r libreasongl.a libhelloworld.o
  ~/.opam/4.04.0+32bit/android-ndk/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-ranlib libreasongl.a
  cp libreasongl.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
  # cp libasmrun.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
fi


if [ -z "olddddd 64 fakery" ]; then
  opam switch 4.04.0+32bit
  eval `opam config env`
  cp android-sysroot-arm64/lib/ocaml/libasmrun.a ./
  echo "static compile now"
  ocamlfind -toolchain android ocamlopt -linkpkg -output-obj -cclib -shared helloworld.ml -o libhelloworld.o
  cp libasmrun.a libreasongl.a
  ar -r libreasongl.a libhelloworld.o
  # cp libhelloworld.o libreasongl.o
  ~/.opam/4.04.0+32bit/android-ndk/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-ranlib libreasongl.a
  cp libreasongl.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
  # cp libreasongl.o android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
  # cp libasmrun.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
  # cp libasmrun.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
fi


# Links

# http://www.learnopengles.com/developing-a-simple-game-of-air-hockey-using-c-and-opengl-es-2-for-android-ios-and-the-web/
# http://www.learnopengles.com/calling-opengl-from-android-using-the-ndk/

if [ -z "waaaa" ]; then
  echo "static compile maybe - complete-object"
  cp 32bit/android-sysroot/lib/ocaml/libasmrun.a ./
  ocamlfind -toolchain android ocamlopt -g -linkpkg \
    -ccopt -fno-omit-frame-pointer \
    -linkall \
    -ccopt -fPIC \
    -runtime-variant _pic \
    -output-complete-obj \
    -output-obj \
    -verbose   \
    helloworld.ml -o libhelloworld.o
  cp libhelloworld.o android-ndk/hello-libs/distribution/reasongl/lib/armeabi-v7a
  cp libasmrun.a android-ndk/hello-libs/distribution/reasongl/lib/armeabi-v7a

  # cp libasmrun.a libreasongl.a
  # ar -r libreasongl.a libhelloworld.o
  # ~/.opam/4.04.0+32bit/android-ndk/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-ranlib libreasongl.a
  # cp libreasongl.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
fi


if [ -z "aa" ]; then
  echo "static compile now"
  ocamlfind -toolchain android ocamlopt -linkpkg -output-obj -cclib -shared helloworld.ml -o libhelloworld.o
  cp libasmrun.a libreasongl.a
  ar -r libreasongl.a libhelloworld.o
  # cp libhelloworld.o libreasongl.o
  ~/.opam/4.04.0+32bit/android-ndk/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-ranlib libreasongl.a
  cp libreasongl.a android-ndk/hello-libs/distribution/reasongl/lib/armeabi-v7a
  # cp libreasongl.o android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
  # cp libasmrun.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
  # cp libasmrun.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
fi


if [ -z "" ]; then
  opam switch 4.04.0-android32
  eval `opam config env`
  echo "static compile maybe"
  cp 32bit/android-sysroot/lib/ocaml/libasmrun.a ./
  ocamlfind -toolchain android ocamlopt -linkpkg -output-obj helloworld.ml libasmrun.a 32bit/android-sysroot/lib/ocaml/stdlib.a -o libhelloworld.o
  cp libasmrun.a libreasongl.a
  ar -r libreasongl.a libhelloworld.o
  ~/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-ranlib libreasongl.a
  mkdir -p demos/hello-libs/distribution/reasongl/lib/armeabi-v7a
  cp libreasongl.a demos/hello-libs/distribution/reasongl/lib/armeabi-v7a/
  # cp libasmrun.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
fi

