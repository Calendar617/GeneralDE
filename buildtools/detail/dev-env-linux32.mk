CPPFLAGS+=-I$(CPDE_ROOT)/include

ifneq ($(DEBUG),0)
CFLAGS+=-ggdb
CXXFLAGS+=-ggdb
endif

LDFLAGS.share:=--shared -z defs

