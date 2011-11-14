product:=gtest.cpe_dr
$(product).sources := $(wildcard $(product-base)*.cpp)
$(product).type:=cppprogn
$(product).buildfor:=dev
$(product).depends:=gtest cpe_utils cpe_dr
$(product).flags.ld:=
$(product).run.path:=$(product-base)
cpe_dr.ut:=$(product)

$(eval $(call product-def,$(product)))
