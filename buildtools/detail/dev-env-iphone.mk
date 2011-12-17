# {{{ global settings
self-dev-env:=iphone

CFLAGS+=-Wall
CXXFLAGS+=-Wall

ifneq ($(DEBUG),0)
CFLAGS+=-ggdb
CXXFLAGS+=-ggdb
endif

LDFLAGS.share:=--shared

$(self-dev-env).default-lib-type:=static

dep.iconv?=system
# }}}
# {{{ validate
$(call assert-not-null,PLATFORM_NAME)
$(call assert-not-null,PLATFORM_VERSION)
# }}}
# {{{ sdk iPhoneSimulator

iphone-os-version-min?=40200

iPhoneSimulator.compiler ?= llvm-$1-4.2

iPhoneSimulator.TARGET_ARCH ?= -arch i386

iPhoneSimulator.CPPFLAGS ?= -D__IPHONE_OS_VERSION_MIN_REQUIRED=$(iphone-os-version-min)

iPhoneSimulator.CFLAGS ?= \
                   -pipe\
                   -std=c99 \
                   -Os \
                   -fexceptions \
                   -fvisibility=hidden \
                   -mmacosx-version-min=10.6 \
                   -gdwarf-2 \

iPhoneSimulator.CFLAGS.m ?= \
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

PLATFORM_PREFIX:=/Developer/Platforms/$(PLATFORM_NAME).platform
PLATFORM_BIN_PATH:=$(PLATFORM_PREFIX)/Developer/usr/bin
SDK_PREFIX:=$(PLATFORM_PREFIX)/Developer/SDKs/$(PLATFORM_NAME)$(PLATFORM_VERSION).sdk

$(call assert-not-null,$(PLATFORM_NAME).compiler)
$(call assert-not-null,$(PLATFORM_NAME).CFLAGS)
$(call assert-not-null,$(PLATFORM_NAME).CFLAGS.m)
$(call assert-not-null,$(PLATFORM_NAME).LDFLAGS)
$(call assert-not-null,$(PLATFORM_NAME).TARGET_ARCH)

CC = $(GCC)
GCC = $(PLATFORM_BIN_PATH)/$(call $(PLATFORM_NAME).compiler,gcc)
CXX = $(PLATFORM_BIN_PATH)/$(call $(PLATFORM_NAME).compiler,g++)
LD = $(CC)
AR = $(PLATFORM_BIN_PATH)/ar
STRIP = $(PLATFORM_BIN_PATH)/strip
OBJCOPY = $(PLATFORM_BIN_PATH)/objcopy
IBTOOL = ibtool

IBFLAGS = --errors \
          --warnings \
          --notices \
          --output-format human-readable-text \
          --sdk $(SDK_PREFIX)

CPPFLAGS+= -isysroot $(SDK_PREFIX) \
           -F$(SDK_PREFIX)/System/Library/Frameworks \
           $(addprefix -I,$(sort $(dir $(sources)))) \
           $(INCLUDES) \
           $($(PLATFORM_NAME).CPPFLAGS)

CFLAGS += $($(PLATFORM_NAME).CFLAGS) \

CFLAGS.m += $($(PLATFORM_NAME).CFLAGS.m)

TARGET_ARCH += $($(PLATFORM_NAME).TARGET_ARCH)

LDFLAGS += -isysroot $(SDK_PREFIX) \
           $($(PLATFORM_NAME).LDFLAGS) \
           $(addprefix -L,$(sort $(dir $(libraries)))) \

PLUTILFLAGS += -convert binary1

# }}}
