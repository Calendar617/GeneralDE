dev-env-list+=linux64

linux64.GCC?=$(shell which gcc)
linux64.CC=$(linux64.GCC)
linux64.CXX?=$(shell which g++)
linux64.AR?=$(shell which ar)

linux64.CFLAGS+=-Wall -m64 -fPIC
linux64.CXXFLAGS+=-Wall -m64 -fPIC

linux64.linker.c:=$(linux64.GCC)
linux64.linker.cpp:=$(linux64.CXX)
linux64.linker.obj-c:=$(linux64.GCC)
linux64.linker.obj-cpp=$(linux64.CXX)

ifneq ($(DEBUG),0)
linux64.CFLAGS+=-ggdb
linux64.CXXFLAGS+=-ggdb
endif

linux64.LDFLAGS:=-z defs
linux64.LDFLAGS.share:=--shared -z defs

linux64.default-lib-type:=dynamic
linux64.make-static-lib-name=lib$1.a
linux64.make-dynamic-lib-name=lib$1.so
linux64.make-executable-name=$1
linux64.export-symbols=$(addprefix -u, $1)

linux64.lib.iconv:=

linux64.lib.math?=m
linux64.lib.dl?=dl