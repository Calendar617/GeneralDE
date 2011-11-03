product:=gtest.cpe_mem
$(product).sources := $(wildcard $(product-base)*.cpp)
$(product).type:=cppprogn
$(product).buildfor:=dev
$(product).depends:=gtest cpe_mem
$(product).flags.ld:=
$(product).run.path:=$(product-base)
cpe_mem.ut:=$(product)

$(eval $(call product-def,$(product)))
