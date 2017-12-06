
#
# Copyright (C) The Android Open Source Project
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

cmake_minimum_required(VERSION 3.4.1)

# configure import libs
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../../../../distribution)

add_library(lib_gmath STATIC IMPORTED)
set_target_properties(lib_gmath PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/gmath/lib/${ANDROID_ABI}/libgmath.a)

# shared lib will also be tucked into APK and sent to target
# refer to app/build.gradle, jniLibs section for that purpose.
# ${ANDROID_ABI} is handy for our purpose here. Probably this ${ANDROID_ABI} is
# the most valuable thing of this sample, the rest are pretty much normal cmake

add_library(lib_gperf SHARED IMPORTED)
set_target_properties(lib_gperf PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/gperf/lib/${ANDROID_ABI}/libgperf.so)


add_library(lib_reasongl STATIC IMPORTED)
set_target_properties(lib_reasongl PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/reasongl/lib/${ANDROID_ABI}/libhelloworld.o)


add_library(lib_asmrun STATIC IMPORTED)
set_target_properties(lib_asmrun PROPERTIES IMPORTED_LOCATION
    ${distribution_DIR}/reasongl/lib/${ANDROID_ABI}/libasmrun.a)


# add_library(lib_reasongl SHARED IMPORTED)
# set_target_properties(lib_reasongl PROPERTIES IMPORTED_LOCATION
#     ${distribution_DIR}/reasongl/lib/${ANDROID_ABI}/libreasongl.so)
#
# add_library(lib_asmrun SHARED IMPORTED)
# set_target_properties(lib_asmrun PROPERTIES IMPORTED_LOCATION
#    ${distribution_DIR}/reasongl/lib/${ANDROID_ABI}/libasmrun_shared.so)



# build application's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_library(hello-libs SHARED
            hello-libs.cpp)

target_include_directories(hello-libs PRIVATE
                           ${distribution_DIR}/gmath/include
                           ${distribution_DIR}/gperf/include
                           ${distribution_DIR}/../../../android-sysroot-arm64/lib/ocaml
                           ${distribution_DIR}/reasongl/include)

target_link_libraries(hello-libs
                      android
                      lib_gmath
                      lib_gperf
                      lib_reasongl
                      lib_asmrun
                      log)
