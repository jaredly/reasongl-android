eval `opam config env`
set -ex


# /Users/jared/.opam/4.04.0-android32/android-sysroot/bin/ocamlopt -g -ccopt -fPIC -ccopt -pie -o helloworld.native -verbose helloworld.ml
# /Users/jared/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-as -o 'helloworld.o' $S_FILE
# /Users/jared/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-as -o $O_FILE $S_FILE

/Users/jared/.opam/4.04.0-android32/android-sysroot/bin/ocamlopt -g -output-obj -o helloworld.native.o helloworld.ml

READELF=~/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-readelf
$READELF --dynamic helloworld.native.o | grep TEXTREL || echo "No Textrel Found in .o"
$READELF -d '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/std_exit.o' | grep TEXTREL || echo "Not in std_exit"
$READELF -d '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/stdlib.a' | grep TEXTREL || echo "Not in stdlib"
$READELF -d '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/libasmrun.a' | grep TEXTREL || echo "Not in libasmrun"

###### CONCLUSION ######
# per https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81498
# it looks like the ability to have BOTH "pie" and "no dynamic relocatiosn" is
# relatively new (added in sept 2017), so not in this toolchain.
# Hopefully I don't actually need these things for a shared lib, but we'll
# see.

/Users/jared/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-gcc \
  --sysroot /Users/jared/.opam/4.04.0-android32/android-ndk/platforms/android-24/arch-arm -I/Users/jared/.opam/4.04.0-android32/android-ndk/include \
  -L/Users/jared/.opam/4.04.0-android32/android-ndk/lib -I/Users/jared/.opam/4.04.0-android32/android-ndk/sources/cxx-stl/gnu-libstdc++/4.9/include \
  -I/Users/jared/.opam/4.04.0-android32/android-ndk/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi/include \
  -L/Users/jared/.opam/4.04.0-android32/android-ndk/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi \
  -I/Users/jared/.opam/4.04.0-android32/android-sysroot/include \
  -L/Users/jared/.opam/4.04.0-android32/android-sysroot/lib \
  -O2 -o 'helloworld.native'   \
  '-L/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml' \
  -pie \
  '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/std_exit.o' \
  'helloworld.native.o' \
  '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/stdlib.a' \
  '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/libasmrun.a' -lm  -ldl -z text

~/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-readelf --dynamic helloworld.native | grep TEXTREL || echo "No TEXTREL found"
~/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-readelf -r helloworld.native



# export OCAMLPARAM="cclib=-fPIE,-pie"
# ./android-sysroot/bin/ocamlopt -g -ccopt -fPIE -ccopt -pie -ccopt -isysroot -no-alias-deps -I ./android-sysroot/lib/ocaml -nostdlib helloworld.ml -o helloworld.native ./android-sysroot/lib/ocaml/libasmrun.a ./android-sysroot/lib/ocaml/stdlib.a ./android-sysroot/lib/ocaml/std_exit.o

# (this one) ocamlfind -toolchain android ocamlopt -g -ccopt -fPIC -ccopt -pie helloworld.ml -o helloworld.native -verbose

# ocamlfind -toolchain android ocamlopt -g -ccopt -pie -ccopt -fpic -ccopt -fPIC -ccopt -fPIE helloworld.ml -o helloworld.native -verbose
# ocamlfind -toolchain android ocamlopt -g helloworld.ml -o helloworld.native -verbose
adb push helloworld.native /data/local/tmp/helloworld.native
adb shell chmod +x /data/local/tmp/helloworld.native
echo "running"
adb shell /data/local/tmp/helloworld.native






# + /Users/jared/.opam/4.04.0-android32/android-sysroot/bin/ocamlopt -g -ccopt -pie -ccopt -fPIE -o helloworld.native -verbose helloworld.ml
# + /Users/jared/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-as -o 'helloworld.o' '/var/folders/fg/g6sqq3z109zcgm6t8lg1h3gc0000gp/T/camlasmb58226.s'
# + /Users/jared/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-as -o '/var/folders/fg/g6sqq3z109zcgm6t8lg1h3gc0000gp/T/camlstartupc074e6.o' '/var/folders/fg/g6sqq3z109zcgm6t8lg1h3gc0000gp/T/camlstartupbe5e4a.s'
# + /Users/jared/.opam/4.04.0-android32/android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-gcc --sysroot /Users/jared/.opam/4.04.0-android32/android-ndk/platforms/android-24/arch-arm -I/Users/jared/.opam/4.04.0-android32/android-ndk/include -L/Users/jared/.opam/4.04.0-android32/android-ndk/lib -I/Users/jared/.opam/4.04.0-android32/android-ndk/sources/cxx-stl/gnu-libstdc++/4.9/include -I/Users/jared/.opam/4.04.0-android32/android-ndk/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi/include -L/Users/jared/.opam/4.04.0-android32/android-ndk/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi -I/Users/jared/.opam/4.04.0-android32/android-sysroot/include -L/Users/jared/.opam/4.04.0-android32/android-sysroot/lib -O2 -o 'helloworld.native'   '-L/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml' -pie -fPIE '/var/folders/fg/g6sqq3z109zcgm6t8lg1h3gc0000gp/T/camlstartupc074e6.o' '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/std_exit.o' 'helloworld.o' '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/stdlib.a' '/Users/jared/.opam/4.04.0-android32/android-sysroot/lib/ocaml/libasmrun.a' -lm  -ldl


# Debugging the text relocations thing
