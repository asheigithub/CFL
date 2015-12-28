
APP_ABI := armeabi armeabi-v7a x86 mips arm64-v8a x86_64 mips64

APP_PLATFORM := android-9

APP_STL := gnustl_static

#NDK_TOOLCHAIN_VERSION := clang
#LOCAL_CPPFLAGS += -std=c++11

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char -pthread -fexceptions