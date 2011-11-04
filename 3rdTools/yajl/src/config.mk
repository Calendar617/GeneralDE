product:=yajl
$(product).type:=clib
$(product).sources:=$(wildcard $(product-base)*.c)
$(product).product.include:=3rdTools/yajl/include
$(product).flags.cpp:=-I$(product-base)/$(dev-env) -DNDEBUG -DYAJL_SHARED -DYAJL_BUILD \
                      $(if $(filter linux32,$(dev-env)),-DLLONG_MAX=LONG_MAX -DLLONG_MIN=LONG_MIN)
$(product).flags.c:= -O2
$(product).flags.ld:=

$(eval $(call product-def,$(product)))
