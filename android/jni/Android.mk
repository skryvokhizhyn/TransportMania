
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libSDL2 
LOCAL_SRC_FILES := ../../3rdparty/sdl2/lib/$(TARGET_ARCH_ABI)/libSDL2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libBoostSystem 
LOCAL_SRC_FILES := $(BOOST_PATH)/lib32-gcc/libboost_system-gcc-mt-s-1_55.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libBoostFilesystem 
LOCAL_SRC_FILES := $(BOOST_PATH)/lib32-gcc/libboost_filesystem-gcc-mt-s-1_55.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := TransportManiaNative

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../src/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../3rdparty/SDL2/include
LOCAL_C_INCLUDES += $(BOOST_PATH)/

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
		$(wildcard $(LOCAL_PATH)/*.cpp) \
		$(wildcard $(LOCAL_PATH)/*.c) \
		$(wildcard $(LOCAL_PATH)/../../src/*.cpp) \
	)

LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid 
LOCAL_STATIC_LIBRARIES := libSDL2 libBoostSystem libBoostFilesystem

include $(BUILD_SHARED_LIBRARY)


