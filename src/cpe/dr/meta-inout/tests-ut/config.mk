product:=gtest.cpe_dr_meta_inout
$(product).sources := $(wildcard $(product-base)*.cpp)
$(product).type:=cppprogn
$(product).buildfor:=dev
$(product).depends:=gtest cpe_utils cpe_dr cpe_dr_meta_inout xml2
$(product).flags.ld:=
$(product).run.path:=$(product-base)
cpe_dr_meta_inout.ut:=$(product)

$(eval $(call product-def,$(product)))
