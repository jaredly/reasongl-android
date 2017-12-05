
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


# latest attempt
if [ -z "" ]; then
  echo "static compile now"
  ocamlfind -toolchain android ocamlopt -linkpkg -output-complete-obj helloworld.ml libasmrun.a -o libhelloworld.o
  # cp libasmrun.a libreasongl.a
  # ar -r libreasongl.a libhelloworld.o
  cp libhelloworld.o libreasongl.o
  ~/.opam/4.04.0+32bit/android-ndk/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin/aarch64-linux-android-ranlib libreasongl.o
  cp libreasongl.o android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
  # cp libasmrun.a android-ndk/hello-libs/distribution/reasongl/lib/arm64-v8a
fi

