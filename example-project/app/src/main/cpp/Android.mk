LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# config distributed lib path
EXT_LIB_ROOT := $(LOCAL_PATH)/../../../../distribution

include $(CLEAR_VARS)
LOCAL_MODULE := local_reasongl
LOCAL_SRC_FILES := $(EXT_LIB_ROOT)/reasongl/lib/$(TARGET_ARCH_ABI)/libreasongl.a
LOCAL_EXPORT_C_INCLUDES := $(EXT_LIB_ROOT)/reasongl/include
include $(PREBUILT_STATIC_LIBRARY)

# build our app's shared lib
include $(CLEAR_VARS)
LOCAL_CFLAGS := -std=gnu++11
LOCAL_MODULE    := hello-libs
LOCAL_SRC_FILES := hello-libs.cpp
LOCAL_LDLIBS    := -llog -landroid
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_STATIC_LIBRARIES := local_reasongl

include $(BUILD_SHARED_LIBRARY)

