self-dev-env=linux32

GCC?=$(shell which gcc)
CC=$(GCC)

CFLAGS+=-Wall
CXXFLAGS+=-Wall

ifneq ($(DEBUG),0)
CFLAGS+=-ggdb
CXXFLAGS+=-ggdb
endif

LDFLAGS.share:=--shared -z defs

$(self-dev-env).default-lib-type:=dynamic