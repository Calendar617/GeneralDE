product:=cpepp_dr
$(product).type:=lib
$(product).depends:=cpepp_utils cpe_dr
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
