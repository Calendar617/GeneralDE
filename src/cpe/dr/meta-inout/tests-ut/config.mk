product:=gtest.cpe_dr_meta_inout
$(product).type:=progn
$(product).buildfor:=dev
$(product).depends:=gtest cpe_utils cpe_dr cpe_dr_meta_inout xml2
$(product).c.sources := $(wildcard $(product-base)*.cpp)
$(product).flags.ld:=
$(product).run.path:=$(product-base)
cpe_dr_meta_inout.ut:=$(product)

$(eval $(call product-def,$(product)))
