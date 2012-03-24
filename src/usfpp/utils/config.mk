product:=usfpp_utils
$(product).type:=lib
$(product).depends:=cpepp_utils gdpp_app usf_utils
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
