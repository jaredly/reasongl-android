
DEMO_DEST=example-project/distribution/reasongl/lib/armeabi-v7a/libreasongl.a
APK=example-project/app/build/outputs/apk/app-debug.apk

full: $(APK)

$(APK): $(DEMO_DEST)
	cd example-project && ./gradlew assembleDebug

install: $(DEMO_DEST)
	cd example-project && ./gradlew installDebug

$(DEMO_DEST): build/libreasongl.o
	cp $(SYSROOT)/lib/ocaml/libasmrun $(DEMO_DEST)
	ar -r $(DEMO_DEST) build/libreasongl.o

build/libreasongl.o: build/src/main.ml
	ocamlfind -toolchain android ocamlopt -output-obj build/src/main.ml -o build/libreasongl.o

build/src/main.ml: src/main.ml
	mkdir -p build/src
	cp src/main.ml build/src/main.ml

