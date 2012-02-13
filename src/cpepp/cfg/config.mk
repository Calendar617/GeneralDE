product:=cpepp_cfg
$(product).type:=lib
$(product).depends:=cpe_cfg
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
