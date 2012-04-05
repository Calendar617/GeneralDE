product:=cpe_dr_data_basic
$(product).type:=lib
$(product).depends:=cpe_dr cpe_utils
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
