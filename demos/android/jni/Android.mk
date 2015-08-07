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

SOLOUD_FILES	:= $(wildcard $(LOCAL_PATH)/../../../src/audiosource/monotone/*.cpp) 
SOLOUD_FILES	+= $(wildcard $(LOCAL_PATH)/../../../src/audiosource/speech/*.cpp)
SOLOUD_FILES	+= $(wildcard $(LOCAL_PATH)/../../../src/audiosource/sfxr/*.cpp)
SOLOUD_FILES	+= $(wildcard $(LOCAL_PATH)/../../../src/audiosource/wav/*.cpp)
SOLOUD_FILES	+= $(wildcard $(LOCAL_PATH)/../../../src/audiosource/wav/*.c)
SOLOUD_FILES	+= $(wildcard $(LOCAL_PATH)/../../../src/backend/opensles/*.cpp)
SOLOUD_FILES	+= $(wildcard $(LOCAL_PATH)/../../../src/core/*.cpp)
SOLOUD_FILES	+= $(wildcard $(LOCAL_PATH)/../../../src/filter/*.cpp)
SOLOUD_FILES	:= $(SOLOUD_FILES:$(LOCAL_PATH)/%=%)

LOCAL_MODULE    := soloud-android
LOCAL_SRC_FILES := main.cpp
LOCAL_SRC_FILES += $(SOLOUD_FILES)

LOCAL_CPPFLAGS	+= -DWITH_OPENSLES=1

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
