product:=gd_app
$(product).type:=lib
$(product).depends:=cpe_utils cpe_cfg gd_tl gd_dp gd_nm cpe_net
$(product).c.libraries:=dl
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).product.c.libraries:=$($(product).c.libraries)
$(product).product.c.flags.ld:=$($(product).c.flags.ld)

$(eval $(call product-def,$(product)))