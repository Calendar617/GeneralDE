self-dev-env=vc

$(call assert-not-null,VC_HOME)
$(call assert-file-exists,$(VC_HOME))

GCC?=$(shell which gcc)
CC=$(GCC)

CFLAGS+=-Wall
CXXFLAGS+=-Wall

ifneq ($(DEBUG),0)
CFLAGS+=-ggdb
CXXFLAGS+=-ggdb
endif

LDFLAGS.share:=--shared

dep.iconv?=system

$(self-dev-env).default-lib-type:=dynamic
