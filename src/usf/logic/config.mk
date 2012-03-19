product:=usf_logic
$(product).type:=lib
$(product).depends:=cpe_utils cpe_cfg cpe_dr_data_basic cpe_dr_data_cfg gd_app
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
