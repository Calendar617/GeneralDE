product:=gmock
$(product).type:=lib
$(product).depends:=gtest
$(product).buildfor:=dev
$(product).product.c.includes:=3rdTools/gmock/include
$(product).c.flags.cpp:=-DHAVE_CONFIG_H
$(product).c.sources:=$(wildcard $(product-base)*.cc)
$(product).c.includes:=3rdTools/gmock
$(product).c.flags.ld:=-lpthread
$(product).c.linker:=$(LINK.cc)

$(eval $(call product-def,$(product)))
