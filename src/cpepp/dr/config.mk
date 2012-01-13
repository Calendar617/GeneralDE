product:=cpepp_dr
$(product).type:=lib
$(product).depends:=cpepp_utils cpe_dr cpe_dr_meta_inout
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
