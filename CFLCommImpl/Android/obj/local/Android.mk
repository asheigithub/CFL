LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := CFLCommImpl
LOCAL_MODULE_FILENAME := libCFLCommImpl
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libCFLCommImpl.a 
				

#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
                       -llog \
                       -landroid \
					   -lEGL 

include $(PREBUILT_STATIC_LIBRARY)
