# {{{ global settings
dev-env-list+=iphone

iphone.CFLAGS+=-Wall
iphone.CXXFLAGS+=-Wall
iphone.MFLAGS+=-Wall -pipe -x objective-c
iphone.MMFLAGS+=-Wall -pipe -x objective-c++

ifneq ($(DEBUG),0)
iphone.CFLAGS+=-ggdb
iphone.CXXFLAGS+=-ggdb
endif

iphone.LDFLAGS.share:=--shared

iphone.default-lib-type:=static
iphone.make-static-lib-name=lib$1.a
iphone.make-dynamic-lib-name=lib$1.so
iphone.make-executable-name=$1

# }}}
# {{{ validate
iphone.check=$(call assert-not-null,PLATFORM_NAME) \
             $(call assert-not-null,PLATFORM_VERSION) \
             $(call assert-not-null,$(PLATFORM_NAME).compiler) \
             $(call assert-not-null,$(PLATFORM_NAME).CFLAGS) \
             $(call assert-not-null,$(PLATFORM_NAME).CXXFLAGS) \
             $(call assert-not-null,$(PLATFORM_NAME).MFLAGS) \
             $(call assert-not-null,$(PLATFORM_NAME).MMFLAGS) \
             $(call assert-not-null,$(PLATFORM_NAME).LDFLAGS) \
             $(call assert-not-null,$(PLATFORM_NAME).TARGET_ARCH) \
             $(call assert-file-exists,$(SDK_PREFIX))

# }}}
# {{{ sdk iPhoneSimulator

iphone-os-version-min?=40200

iPhoneSimulator.compiler ?= llvm-$1-4.2

iPhoneSimulator.TARGET_ARCH ?= -arch i386

iPhoneSimulator.CPPFLAGS ?= \
                    -D__IPHONE_OS_VERSION_MIN_REQUIRED=$(iphone-os-version-min) \
                    -DTARGET_IPHONE_SIMULATOR

iPhoneSimulator.CFLAGS ?= \
                   -std=c99 \
                   -fexceptions \
                   -mmacosx-version-min=10.6 \
                   -gdwarf-2 \

iPhoneSimulator.CXXFLAGS ?= \
                   -fexceptions \
                   -mmacosx-version-min=10.6 \
                   -gdwarf-2 \

iPhoneSimulator.MFLAGS ?= \
                   $(iPhoneSimulator.CFLAGS) \
                   -fmessage-length=0 \
                   -fpascal-strings \
                   -fasm-blocks \
                   -fobjc-abi-version=2 \
                   -fobjc-legacy-dispatch \

iPhoneSimulator.MMFLAGS ?= \
                   -fmessage-length=0 \
                   -fpascal-strings \
                   -fasm-blocks \
                   -fobjc-abi-version=2 \
                   -fobjc-legacy-dispatch \

iPhoneSimulator.LDFLAGS ?=  -arch i386\
                            -mmacosx-version-min=10.6 \
                            -Xlinker \
                            -objc_abi_version \
                            -Xlinker 2 

iPhoneSimulator.install-dir?=$(HOME)/Library/Application Support/iPhone Simulator/$(PLATFORM_VERSION)/Applications
iPhoneSimulator.run := 	echo ' \
application "iPhone Simulator" quit\n \
application "iPhone Simulator" activate\n \
' | osascript

# }}}
# {{{ toolset def

IOS_XCODE_ROOT:=$(if $(filter mac,$(OS_NAME)),$(shell xcode-select -print-path))
PLATFORM_PREFIX:=$(IOS_XCODE_ROOT)/Platforms/$(PLATFORM_NAME).platform
PLATFORM_BIN_PATH:=$(PLATFORM_PREFIX)/Developer/usr/bin
SDK_PREFIX:=$(PLATFORM_PREFIX)/Developer/SDKs/$(PLATFORM_NAME)$(PLATFORM_VERSION).sdk

iphone.GCC = $(PLATFORM_BIN_PATH)/$(call $(PLATFORM_NAME).compiler,gcc)
iphone.CXX = $(PLATFORM_BIN_PATH)/$(call $(PLATFORM_NAME).compiler,g++)
iphone.CC = $(iphone.GCC)
iphone.LD = $(iphone.CC)
iphone.AR = $(PLATFORM_BIN_PATH)/ar
iphone.STRIP = $(PLATFORM_BIN_PATH)/strip
iphone.OBJCOPY = $(PLATFORM_BIN_PATH)/objcopy
iphone.IBTOOL = ibtool

iphone.IBFLAGS = \
          --errors \
          --warnings \
          --notices \
          --output-format human-readable-text \
          --sdk $(SDK_PREFIX)

iphone.CPPFLAGS+=\
           -isysroot $(SDK_PREFIX) \
           -F$(SDK_PREFIX)/System/Library/Frameworks \
           $($(PLATFORM_NAME).CPPFLAGS)

iphone.CFLAGS += $($(PLATFORM_NAME).CFLAGS)
iphone.CXXFLAGS += $($(PLATFORM_NAME).CXXFLAGS)
iphone.MFLAGS += $($(PLATFORM_NAME).MFLAGS)
iphone.MMFLAGS += $($(PLATFORM_NAME).MMFLAGS)

iphone.TARGET_ARCH += $($(PLATFORM_NAME).TARGET_ARCH)

iphone.LDFLAGS += \
           -isysroot $(SDK_PREFIX) \
           $($(PLATFORM_NAME).LDFLAGS) \
           $(addprefix -L,$(sort $(dir $(libraries)))) \

iphone.PLUTILFLAGS += -convert binary1

iphone.linker.c:=$(iphone.GCC)
iphone.linker.cpp:=$(iphone.CXX)
iphone.linker.obj-c:=$(iphone.GCC)
iphone.linker.obj-cpp:=$(iphone.CXX)


# }}}
