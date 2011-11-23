product:=yajl
$(product).type:=lib
$(product).product.c.includes:=3rdTools/yajl/include
$(product).c.sources:=$(wildcard $(product-base)*.c)
$(product).c.includes:=3rdTools/yajl/include/src/$(dev-env)
$(product).c.flags.cpp:=-DNDEBUG -DYAJL_SHARED -DYAJL_BUILD \
                        $(if $(filter linux32,$(dev-env)),-DLLONG_MAX=LONG_MAX -DLLONG_MIN=LONG_MIN)
$(product).c.flags.c:= -O2
$(product).c.flags.ld:=

$(eval $(call product-def,$(product)))
