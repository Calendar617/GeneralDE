product:=gd_dr_dm
$(product).type:=lib
$(product).depends:=cpe_utils cpe_dr gd_app gd_dr_store gd_utils
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(eval $(call product-def,$(product),tools))