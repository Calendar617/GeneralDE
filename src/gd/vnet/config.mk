product:=gd_vnet
$(product).type:=lib
$(product).depends:=cpe_utils cpe_cfg gd_app
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
