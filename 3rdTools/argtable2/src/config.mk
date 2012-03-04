product:=argtable2
$(product).type:=lib
$(product).c.libraries:=
$(product).version:=4.0.4
$(product).c.sources := $(wildcard $(product-base)/*.c)
$(product).product.c.includes:=3rdTools/argtable2/include
$(product).c.includes:=3rdTools/argtable2/src/$(dev-env)
$(product).c.flags.cpp:=-DHAVE_CONFIG_H -DNDEBUG
$(product).version:=2.13

$(eval $(call product-def,$(product)))
