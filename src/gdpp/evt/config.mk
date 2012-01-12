product:=gdpp_evt
$(product).type:=lib
$(product).depends:=cpepp_dr gd_evt
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
