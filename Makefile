#
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
PRE=
LOCAL_PATH := $(shell pwd)
CC  = $(PRE)gcc 

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
	src/resamplerate/samplerate.c \
	src/resamplerate/src_linear.c \
	src/resamplerate/src_sinc.c \
	src/resamplerate/src_zoh.c \
	src/iniparser/dictionary.c \
	src/iniparser/iniparser.c \
	test/resamplerate.c

DFU_OBJ                                           := $(patsubst %.c, %.o, ${LOCAL_SRC_FILES})

EXECUTABLE                                = resamplerate

LOCAL_CFLAGS += -O1 -g -fpic -Wall 


LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/include

INCLUDE_FLAGS                             += -I$(LOCAL_C_INCLUDES)

LOCAL_CFLAGS += $(INCLUDE_FLAGS)

%.o : %.c
	$(CC) $(LOCAL_CFLAGS)  -fPIC -c $< -o $@

all: $(EXECUTABLE)
	echo all

$(EXECUTABLE) : $(DFU_OBJ) 
#	echo makefile $(DFU_OBJ)  
	$(CC)  $^ $(LOCAL_CFLAGS) -fPIC -o $@

clean:
	rm -rf $(EXECUTABLE) $(DFU_OBJ)

# gcc -Wall -g -O0 -DHAVE_CONFIG_H  -I/home/sunjinzhou/hid_usb/   -fPIC -c /home/sunjinzhou/hid_usb/hidapi.c -o /home/sunjinzhou/hid_usb/hidapi.o
# gcc -Wall -g -O0 -DHAVE_CONFIG_H  -I/home/sunjinzhou/hid_usb/   -fPIC -c /home/sunjinzhou/hid_usb/taihang_hid.c -o /home/sunjinzhou/hid_usb/taihang_hid.o
# gcc -Wall -g -O0 -DHAVE_CONFIG_H  -I/home/sunjinzhou/hid_usb/   -fPIC -c /home/sunjinzhou/hid_usb/th_protocol.c -o /home/sunjinzhou/hid_usb/th_protocol.o
# gcc -Wall -g -O0 -DHAVE_CONFIG_H  -I/home/sunjinzhou/hid_usb/   -fPIC -c /home/sunjinzhou/hid_usb/taihang_uac.c -o /home/sunjinzhou/hid_usb/taihang_uac.o
# gcc -Wall -g -O0 -DHAVE_CONFIG_H  -I/home/sunjinzhou/hid_usb/   -fPIC -c /home/sunjinzhou/hid_usb/taihang_download.c -o /home/sunjinzhou/hid_usb/taihang_download.o
# gcc -Wall -g -O0 -DHAVE_CONFIG_H  -I/home/sunjinzhou/hid_usb/   -fPIC -c /home/sunjinzhou/hid_usb/main.c -o /home/sunjinzhou/hid_usb/main.o
# echo 1111111111 /home/sunjinzhou/hid_usb/hidapi.o  /home/sunjinzhou/hid_usb/taihang_hid.o  /home/sunjinzhou/hid_usb/th_protocol.o  /home/sunjinzhou/hid_usb/taihang_uac.o  /home/sunjinzhou/hid_usb/taihang_download.o  /home/sunjinzhou/hid_usb/main.o libusb-1.0.a
# 1111111111 /home/sunjinzhou/hid_usb/hidapi.o /home/sunjinzhou/hid_usb/taihang_hid.o /home/sunjinzhou/hid_usb/th_protocol.o /home/sunjinzhou/hid_usb/taihang_uac.o /home/sunjinzhou/hid_usb/taihang_download.o /home/sunjinzhou/hid_usb/main.o libusb-1.0.a
# g++ /home/sunjinzhou/hid_usb/hidapi.o /home/sunjinzhou/hid_usb/taihang_hid.o /home/sunjinzhou/hid_usb/th_protocol.o /home/sunjinzhou/hid_usb/taihang_uac.o
# /home/sunjinzhou/hid_usb/taihang_download.o /home/sunjinzhou/hid_usb/main.o 
# libusb-1.0.a  -pie -fPIE -pthread  -fPIC -ludev   -Wall -g -O0 -DHAVE_CONFIG_H  
# -I/home/sunjinzhou/hid_usb/  -fPIC -pthread -o taihang_hid
# ~                                            