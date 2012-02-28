product:=usf_Logic
$(product).type:=lib
$(product).depends:=cpepp_utils gdpp_app
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
