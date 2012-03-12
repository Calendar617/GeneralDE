product:=json-c
$(product).type:=lib
$(product).product.c.includes:=3rdTools/json-c/include
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).c.env-includes:=3rdTools/json-c/src
$(product).c.flags.cpp:=-D_GNU_SOURCE
$(product).c.flags.ld:=

$(eval $(call product-def,$(product)))
