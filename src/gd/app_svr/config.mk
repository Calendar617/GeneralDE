product:=gd_app_net
$(product).type:=lib
$(product).depends:=cpe_utils
$(product).c.libraries:=
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)*.c)
$(product).product.c.libraries:=$($(product).c.libraries)
$(product).product.c.flags.ld:=$($(product).c.flags.ld)

$(eval $(call product-def,$(product)))
