set -ex

# opam switch 4.04.0-android32 -A 4.04.0+32bit
# eval `opam config env`

# opam pin add -y ocamlbuild https://github.com/ocaml/ocamlbuild.git
# opam pin add -y topkg https://github.com/whitequark/topkg.git

opam remove -y ocaml-android32

# opam remove -y conf-android
ARCH=arm SUBARCH=armv7 SYSTEM=linux_eabi \
  CCARCH=arm TOOLCHAIN=arm-linux-androideabi-4.9 \
  TRIPLE=arm-linux-androideabi LEVEL=24 \
  STLVER=4.9 STLARCH=armeabi \
  opam upgrade -y conf-android

# opam install -y ocaml-android32

# echo "## TRYING IT OUT ##"

# echo 'let () = print_endline "Hello, world!"' >helloworld.ml
# ocamlfind -toolchain android ocamlc -custom helloworld.ml -o helloworld.byte
# ocamlfind -toolchain android ocamlopt helloworld.ml -o helloworld.native


