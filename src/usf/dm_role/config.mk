product:=usf_dm_role
$(product).type:=lib
$(product).depends:=cpe_utils cpe_dr gd_app gd_dr_store
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(eval $(call product-def,$(product),tools))