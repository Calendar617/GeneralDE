product:=yajl
$(product).type:=lib
$(product).product.c.includes:=3rdTools/yajl/include
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).c.env-includes:=3rdTools/yajl/include/src
$(product).c.flags.cpp:=-DNDEBUG -DYAJL_SHARED -DYAJL_BUILD
$(product).c.flags.c:= -O2
$(product).c.flags.ld:=

$(product).linux32.c.flags.cpp:=-DLLONG_MAX=LONG_MAX -DLLONG_MIN=LONG_MIN

$(eval $(call product-def,$(product)))
