product:=gd_net
$(product).type:=lib
$(product).depends:=cpe_utils ev
$(product).c.libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
