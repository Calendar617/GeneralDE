product:=gdpp_dp
$(product).type:=lib
$(product).depends:=cpepp_utils gd_dp
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
