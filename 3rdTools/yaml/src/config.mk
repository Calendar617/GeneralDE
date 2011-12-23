product:=yaml
$(product).type:=lib
$(product).product.c.includes:=3rdTools/yaml/include
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).c.includes:=3rdTools/yaml/src/$(dev-env)
$(product).c.flags.cpp:=-DHAVE_CONFIG_H
$(product).c.flags.c:= -O2
$(product).c.flags.ld:=

$(eval $(call product-def,$(product)))
