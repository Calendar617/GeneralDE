product:=gd_evt
$(product).type:=lib
$(product).depends:=gd_tl cpe_dr cpe_dr_data_basic
$(product).c.libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))