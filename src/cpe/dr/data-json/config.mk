product:=cpe_dr_data_json
$(product).type:=lib
$(product).depends:=cpe_dr cpe_utils yajl
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).c.flags.cpp:=$(if $(filter 64,$(cpe-dr-metalib)),-D CPE_DR_METALIB_SIZE=64)

$(eval $(call product-def,$(product)))
