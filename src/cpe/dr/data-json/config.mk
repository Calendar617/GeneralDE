product:=cpe_dr_data_json
$(product).type:=clib
$(product).depends:=cpe_dr json-c
$(product).sources:=$(wildcard $(product-base)*.c)

$(eval $(call product-def,$(product)))
