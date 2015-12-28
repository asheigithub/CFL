LOCAL_PATH := $(call my-dir)
MY_PROJ := E:/CFL
#$(warning $(MY_PROJ))

include $(MY_PROJ)/CFLCommImpl/Android/obj/local/Android.mk



include $(CLEAR_VARS)


LOCAL_MODULE := TestEntry

LOCAL_SRC_FILES := $(MY_PROJ)/Test/TestEntry.cpp

LOCAL_C_INCLUDES:= $(MY_PROJ)/GLES \
				   $(MY_PROJ)/CFLComm/src

LOCAL_STATIC_LIBRARIES := CFLCommImpl android_native_app_glue boost_atomic_static

LOCAL_LDLIBS := -lGLESv2 -llog -landroid -lEGL

include $(BUILD_SHARED_LIBRARY)


$(call import-module,android/native_app_glue)
$(call import-module,boost/1.58.0)