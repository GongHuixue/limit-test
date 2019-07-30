#shutdownanimation_CommonCFlags += -Wall -Werror


LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#LOCAL_CFLAGS += ${shutdownanimation_CommonCFlags}

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libbinder \
    libcutils \
    liblog \
    libutils \
	libEGL \
	libGLESv2 \
	libgui \
	liblayers_proto \
	libui \
	libutils


LOCAL_SRC_FILES:= \
    LimitTest.cpp

LOCAL_MODULE:= limittest 


ifdef TARGET_32_BIT_SURFACEFLINGER
LOCAL_32_BIT_ONLY := true
endif

include $(BUILD_EXECUTABLE)
