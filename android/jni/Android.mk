# Copyright (C) 2009 The Android Open Source Project
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

LOCAL_MODULE    := libSDL2 
LOCAL_SRC_FILES := ../../3rdparty/sdl2/lib/$(TARGET_ARCH_ABI)/libSDL2.a

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := TransportManiaNative

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../3rdparty/SDL2/include

#LOCAL_SRC_FILES := hello-jni.c TransportManiaNative.cpp
LOCAL_SRC_FILES := SDL_android_main.c TransportManiaNative.cpp

LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid 
#LOCAL_STATIC_LIBRARIES := -l../../3rdparty/SDL2/lib/armeabi-v7a/libSDL2.a
LOCAL_STATIC_LIBRARIES := libSDL2

include $(BUILD_SHARED_LIBRARY)


