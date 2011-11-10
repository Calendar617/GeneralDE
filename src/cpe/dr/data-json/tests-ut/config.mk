product:=gtest.cpe_dr_data_json
$(product).sources := $(wildcard $(product-base)*.cpp)
$(product).type:=cppprogn
$(product).buildfor:=dev
$(product).depends:=gtest cpe_dr cpe_utils cpe_dr_meta_inout cpe_dr_data_json xml2 yajl
$(product).flags.ld:=
$(product).run.path:=$(product-base)
cpe_dr_data_json.ut:=$(product)

$(eval $(call product-def,$(product)))
