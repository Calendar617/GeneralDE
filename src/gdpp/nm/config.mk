product:=gdpp_nm
$(product).type:=lib
$(product).depends:=gd_nm
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
