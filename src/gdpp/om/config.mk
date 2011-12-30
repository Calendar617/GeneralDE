product:=gdpp_om
$(product).type:=lib
$(product).depends:=cpepp_utils gd_om
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
