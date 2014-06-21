
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libSDL2 
LOCAL_SRC_FILES := ../../3rdparty/sdl2/lib/$(TARGET_ARCH_ABI)/libSDL2.a

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := TransportManiaNative

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../3rdparty/SDL2/include

LOCAL_SRC_FILES := SDL_android_main.c
LOCAL_SRC_FILES += $(subst $(LOCAL_PATH)/,, $(wildcard $(LOCAL_PATH)/../../src/*.cpp))

LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid 
LOCAL_STATIC_LIBRARIES := libSDL2

include $(BUILD_SHARED_LIBRARY)


