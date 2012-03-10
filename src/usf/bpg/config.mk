product:=usf_bpg
$(product).type:=lib
$(product).depends:=cpe_utils gd_app usf_logic
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
