product:=cpe_dr_data_json
$(product).type:=clib
$(product).depends:=cpe_dr cpe_utils yajl
$(product).sources:=$(wildcard $(product-base)*.c)

$(eval $(call product-def,$(product)))
