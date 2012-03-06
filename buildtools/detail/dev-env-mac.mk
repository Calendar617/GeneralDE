dev-env-list+=mac

mac.GCC?=$(shell which gcc)
mac.CC=$(mac.GCC)
mac.CXX?=$(shell which g++)
mac.AR?=$(shell which ar)

mac.CFLAGS+=-Wall
mac.CXXFLAGS+=-Wall
mac.MFLAGS+=-Wall -x objective-c -fobjc-abi-version=2 -fobjc-legacy-dispatch
mac.MMFLAGS+=-Wall -x objective-c++ -fobjc-abi-version=2 -fobjc-legacy-dispatch

mac.linker.c:=$(mac.GCC)
mac.linker.cpp:=$(mac.CXX)
mac.linker.obj-c:=$(mac.GCC)
mac.linker.obj-cpp=$(mac.CXX)

ifneq ($(DEBUG),0)
mac.CFLAGS+=-ggdb
mac.CXXFLAGS+=-ggdb
endif

mac.LDFLAGS.share:=--shared

mac.default-lib-type:=dynamic
mac.make-static-lib-name=lib$1.a
mac.make-dynamic-lib-name=lib$1.so
mac.make-executable-name=$1
