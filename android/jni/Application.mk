
# use this to select gcc instead of clang
#NDK_TOOLCHAIN_VERSION := 4.8
# otherwise, the following line select the latest clang version.
# NDK_TOOLCHAIN_VERSION := clang
# enable c++11 extensions in source code
APP_CPPFLAGS += -std=c++11 -fexceptions -frtti
# or use APP_CPPFLAGS := -std=gnu++11
# Following is STL implementation. Can be use _static or _shared version, the last one need a call to loadLibrary BEFORE use the C++ code in the shared library
APP_STL := gnustl_static
#APP_STL := stlport_static

#APP_ABI := armeabi armeabi-v7a x86
#ifneq (,$(findstring Windows, $(OS)))
#  APP_ABI := x86
#else
  APP_ABI := armeabi-v7a
#endif



