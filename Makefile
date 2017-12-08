
BSB_BACKEND = native-ios

CURDIR = $(shell pwd)

# This section is for simulator. Uncomment the next section to build to device
OCAMLDIR = ~/.opam/4.04.0-android32/android-sysroot
OCAMLBIN = $(OCAMLDIR)/bin/ocamlopt

OCAMLOPT=BSB_BACKEND=native-android $(OCAMLBIN) -ccopt -fno-omit-frame-pointer -ccopt -O3 -ccopt -fPIC \
	-I Build/src \
	-I Build/app \
	-I Build/reasongl-interface/src \
	-I Build/reprocessing/src \
	-I $(OCAMLDIR)/lib/ocaml \
	-ppx ./matchenv.ppx \
	-runtime-variant _pic

# CFLAGS = -isysroot $(PLT)$(SDK) -isystem $(OCAMLDIR)/lib/ocaml -DCAML_NAME_SPACE -I$(CURDIR)/OCamlTest/OCamlTest -I$(OCAMLDIR)/lib/ocaml/caml -I$(OCAMLDIR)/lib/ocaml -I$(OCAMLDIR)/../stdlib/ -fno-objc-arc -miphoneos-version-min=$(IOSMINREV)
# OCAMLOPT = BSB_BACKEND=$(BSB_BACKEND) $(OCAMLBIN)/bin/ocamlopt -pp 'refmt --print binary' -ppx ./matchenv.ppx -I $(CURDIR) -no-alias-deps -ccopt -isysroot -ccopt $(PLT)$(SDK)
# MLFLAGS = -c -I Build/src -I Build/app -I Build/reasongl-interface/src -I Build/reprocessing/src -I $(OCAMLDIR)/lib/ocaml bigarray.cmxa

# Libraries
REASONGL_INTERFACE_FILES = RGLConstants RGLEvents RGLInterface ReasonglInterface
REPROCESSING_FILES = Reprocessing_Events Reprocessing_Common Reprocessing_Constants Reprocessing_Matrix Reprocessing_Shaders Reprocessing_Internal Reprocessing_Font Reprocessing_Types Reprocessing_Utils Reprocessing_Hotreload Reprocessing_Env Reprocessing_Draw Reprocessing_ClientWrapper Reprocessing

# Reasongl
C_FILES = CforOCaml CforJava CTgls_new CBindings bigarray_stubs mmap_unix
REASONGL_FILES = Capi MLforJava GLConstants Bindings Tgls_new Reasongl

REFMT=~/.opam/4.04.2/bin/refmt

# The test app
APP_PATHS = app/PurpleRain.re
APP_FILES = $(basename $(notdir $(APP_PATHS)))

C_FILES_PATH=$(addprefix Build/src/, $(addsuffix .o, $(C_FILES)))
RE_FILES_PATH=\
	$(addprefix Build/reasongl-interface/src/, $(addsuffix .cmx, $(REASONGL_INTERFACE_FILES))) \
	$(addprefix Build/src/, $(addsuffix .cmx, $(REASONGL_FILES))) \
	$(addprefix Build/reprocessing/src/, $(addsuffix .cmx, $(REPROCESSING_FILES))) \
	$(addprefix Build/app/, $(addsuffix .cmx, $(APP_FILES)))

app:: TestReason

bigarray.o:
	$(OCAMLDIR)/../android-ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/arm-linux-androideabi/bin/ar -x $(OCAMLDIR)/lib/ocaml/bigarray.a
	mv bigarray.o $(OCAMLDIR)/lib/ocaml/

matchenv.ppx:
	ocamlc -pp '$(REFMT) --print binary' -I +compiler-libs ocamlcommon.cma -impl matchenv/src/index.re -o matchenv.ppx

reason-watch:
	# Brew install watchexec if you don't have it https://github.com/mattgreen/watchexec
	watchexec -w src $(MAKE) TestReason

DEST=example-project/app/src/main/jniLibs/armeabi-v7a

TestReason: matchenv.ppx Build $(C_FILES_PATH) $(RE_FILES_PATH)
		$(OCAMLOPT) -output-obj \
			-ccopt -fPIC -ccopt -pie -ccopt -llog -ccopt -landroid \
			bigarray.cmx \
			-ccopt -lGLESv3 -ccopt -lEGL \
			-o libfrom_ocaml.so \
			$(RE_FILES_PATH) $(C_FILES_PATH) $(OCAMLDIR)/lib/ocaml/libasmrun.a
		mkdir -p $(DEST)
		mv libfrom_ocaml.so $(DEST)/libreasongl.so
		# $(OCAMLOPT) bigarray.cmxa $(C_FILES_PATH) $(RE_FILES_PATH) -output-obj -o Build/re_output.o
		# cp $(OCAMLDIR)/lib/ocaml/libasmrun.a Build/libGobi.a
		# ar -r Build/libGobi.a $(C_FILES_PATH) Build/re_output.o

Build:
	mkdir -p Build/src Build/app Build/reasongl-interface/src Build/reprocessing/src

clean::
		rm -f TestApp *.o *.cm[iox]
		rm -rf Build/src/* Build/app/* Build/Products Build/libGobi.a Build/reasongl-interface/src/* Build/reprocessing/src/*

Build/%.o: %.c
		cp $< Build/$<
		$(OCAMLOPT) -ccopt -std=c11 -c Build/$<
		mv *.o $@

Build/%.cmx: %.re
		cp $< Build/$<
		$(OCAMLOPT) -c -pp "$(REFMT) --print binary" -o $@ -impl Build/$<

depend::
		$(OCAMLBIN)/bin/ocamldep -pp '$(REFMT) --print binary' -ml-synonym .re  -I src -I app src/*.re app/*.re > MLDepend
		# $(CC) $(CFLAGS) $(MFLAGS) -MM *.m > MDepend

-include MLDepend
-include MDepend
