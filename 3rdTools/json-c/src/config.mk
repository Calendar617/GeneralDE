product:=json-c
$(product).type:=clib
$(product).sources:=$(wildcard $(product-base)*.c)
$(product).product.include:=3rdTools/json-c/include
$(product).flags.cpp:=-I$(product-base)/$(dev-env)
$(product).flags.ld:=

$(eval $(call product-def,$(product)))
