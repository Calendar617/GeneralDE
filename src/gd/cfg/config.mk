product:=gd_cfg
$(product).type:=lib
$(product).depends:=cpe_utils cpe_dr
$(product).libraries:=
$(product).c.sources:=$(wildcard $(product-base)*.c)

$(eval $(call product-def,$(product)))
