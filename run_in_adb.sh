eval `opam config env`
# export OCAMLPARAM="cclib=-fPIE,-pie"
ocamlfind -toolchain android ocamlopt -g -ccopt fPIC -ccopt -pie helloworld.ml -o helloworld.native
adb push helloworld.native /data/local/tmp/helloworld.native
adb shell chmod +x /data/local/tmp/helloworld.native
adb shell /data/local/tmp/helloworld.native