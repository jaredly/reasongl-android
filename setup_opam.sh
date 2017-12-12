set -ex

if [ ! -d ~/.opam/4.04.0+android+armv7 ]; then
  opam switch 4.04.0+android+armv7 -A 4.04.0+32bit
  eval `opam config env`

  opam pin add -y ocamlbuild https://github.com/ocaml/ocamlbuild.git
  opam pin add -y topkg https://github.com/whitequark/topkg.git

  opam remove -y conf-android
  ARCH=arm SUBARCH=armv7 SYSTEM=linux_eabi \
    CCARCH=arm TOOLCHAIN=arm-linux-androideabi-4.9 \
    TRIPLE=arm-linux-androideabi LEVEL=24 \
    STLVER=4.9 STLARCH=armeabi \
    opam install -y conf-android

  opam install -y ocaml-android
fi;

if [ ! -d ~/.opam/4.04.0+android+x86 ]; then
  opam switch 4.04.0+android+x86 -A 4.04.0+32bit
  eval `opam config env`

  opam pin add -y ocamlbuild https://github.com/ocaml/ocamlbuild.git
  opam pin add -y topkg https://github.com/whitequark/topkg.git

  ARCH=i386 SUBARCH=default SYSTEM=linux_elf \
    CCARCH=x86 TOOLCHAIN=x86-4.9 \
    TRIPLE=i686-linux-android LEVEL=24 \
    STLVER=4.9 STLARCH=x86 \
    opam install -y conf-android

  opam install -y ocaml-android
fi;
