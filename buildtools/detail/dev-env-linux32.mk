CFLAGS+=-Wall
CXXFLAGS+=-Wall

ifneq ($(DEBUG),0)
CFLAGS+=-ggdb
CXXFLAGS+=-ggdb
endif

LDFLAGS.share:=--shared -z defs

