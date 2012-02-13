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

$(self-dev-env).default-lib-type:=dynamic
$(self-dev-env).make-static-lib-name=$1.lib
$(self-dev-env).make-dynamic-lib-name=$1.dll
$(self-dev-env).make-executable-name=$1.exe
