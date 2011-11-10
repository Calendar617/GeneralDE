product:=gtest.cpe_utils
$(product).sources := $(wildcard $(product-base)*.cpp)
$(product).type:=cppprogn
$(product).buildfor:=dev
$(product).depends:=gtest cpe_utils
$(product).flags.ld:=
$(product).run.path:=$(product-base)
cpe_utils.ut:=$(product)

$(eval $(call product-def,$(product)))
