product:=gdpp_app
$(product).type:=lib
$(product).depends:=gd_app
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
