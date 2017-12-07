LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# config distributed lib path
EXT_LIB_ROOT := $(LOCAL_PATH)/../../../../distribution

include $(CLEAR_VARS)
LOCAL_MODULE := local_reasongl
LOCAL_SRC_FILES := $(EXT_LIB_ROOT)/reasongl/lib/$(TARGET_ARCH_ABI)/libreasongl.so
include $(PREBUILT_SHARED_LIBRARY)

