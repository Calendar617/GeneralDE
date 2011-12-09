product:=gtest.cpe_dr_data_json
$(product).type:=progn
$(product).buildfor:=dev
$(product).depends:=gtest cpe_dr cpe_utils cpe_dr_meta_inout cpe_dr_data_json xml2 yajl
$(product).c.flags.ld:=
$(product).c.sources := $(wildcard $(product-base)*.cpp)
$(product).run.path:=$(product-base)
cpe_dr_data_json.ut:=$(product)

$(eval $(call product-def,$(product)))
