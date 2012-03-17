product:=usf_dr_store
$(product).type:=lib
$(product).depends:=cpe_utils cpe_cfg cpe_dr gd_app
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
