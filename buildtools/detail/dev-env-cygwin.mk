dev-env-list+=cygwin

cygwin.GCC?=$(shell which gcc)
cygwin.CC?=$(shell which gcc)
cygwin.CXX?=$(shell which g++)
cygwin.AR?=$(shell which ar)

cygwin.CFLAGS+=-Wall
cygwin.CXXFLAGS+=-Wall

cygwin.linker.c:=$(cygwin.GCC)
cygwin.linker.cpp:=$(cygwin.CXX)
cygwin.linker.obj-c:=$(cygwin.GCC)
cygwin.linker.obj-cpp=$(cygwin.CXX)

ifneq ($(DEBUG),0)
cygwin.CFLAGS+=-ggdb
cygwin.CXXFLAGS+=-ggdb
endif

cygwin.LDFLAGS.share:=--shared -Xlinker --no-undefined

cygwin.default-lib-type:=static
cygwin.make-static-lib-name=lib$1.a
cygwin.make-dynamic-lib-name=$1.dll
cygwin.make-executable-name=$1.exe
cygwin.export-symbols=$(addprefix -u ,$(foreach m,$1,_$m))

cygwin.lib.iconv?=iconv
cygwin.lib.math?=m
cygwin.lib.dl?=dl
