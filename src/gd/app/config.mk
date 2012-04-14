product:=gd_app
$(product).type:=lib
$(product).depends:=cpe_utils cpe_cfg cpe_tl cpe_dp cpe_nm cpe_net
$(product).product.c.env-libraries:=dl
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).product.c.libraries:=$($(product).c.libraries)
$(product).product.c.flags.ld:=$($(product).c.flags.ld)

$(eval $(call product-def,$(product)))
