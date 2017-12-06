set -ex

opam switch 4.04.0+32bit

eval `opam config env`

opam pin add -y ocamlbuild https://github.com/ocaml/ocamlbuild.git
opam pin add -y topkg https://github.com/whitequark/topkg.git

opam remove -y conf-android
OCAMLPARAM=cclib=-fPIE,-pie \
  ARCH=arm64 SUBARCH=arm64 SYSTEM=linux_eabi \
  CCARCH=arm64 TOOLCHAIN=aarch64-linux-android-4.9 \
  TRIPLE=aarch64-linux-android LEVEL=24 \
  STLVER=4.9 STLARCH=arm64-v8a \
  opam install conf-android

opam install -y ocaml-android

echo
echo "## TRYING IT OUT ##"
echo
echo 'let () = print_endline "Hello, world!"' >helloworld.ml
# ocamlfind -toolchain android ocamlc -custom helloworld.ml -o helloworld.byte
ocamlfind -toolchain android ocamlopt helloworld.ml -o helloworld.native
adb push helloworld.native /data/local/tmp/helloworld.native
adb shell chmod +x /data/local/tmp/helloworld.native
adb shell /data/local/tmp/helloworld.native
