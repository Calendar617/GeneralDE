self-dev-env=mac

GCC?=$(shell which gcc)
CC=$(GCC)

CFLAGS+=-Wall
CXXFLAGS+=-Wall
MFLAGS+=-Wall -x objective-c -fobjc-abi-version=2 -fobjc-legacy-dispatch
MMFLAGS+=-Wall -x objective-c++ -fobjc-abi-version=2 -fobjc-legacy-dispatch

ifneq ($(DEBUG),0)
CFLAGS+=-ggdb
CXXFLAGS+=-ggdb
endif

LDFLAGS.share:=--shared

dep.iconv?=system

$(self-dev-env).default-lib-type:=dynamic
$(self-dev-env).make-static-lib-name=lib$1.a
$(self-dev-env).make-dynamic-lib-name=lib$1.so
$(self-dev-env).make-executable-name=$1
