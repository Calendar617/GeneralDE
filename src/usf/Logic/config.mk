product:=usf_logic
$(product).type:=lib
$(product).depends:=cpepp_utils gd_app
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
