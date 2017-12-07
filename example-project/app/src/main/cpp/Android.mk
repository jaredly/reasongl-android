# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
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

