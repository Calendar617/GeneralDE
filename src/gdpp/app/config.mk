product:=gdpp_app
$(product).type:=lib
$(product).depends:=cpepp_utils cpepp_cfg gd_app gdpp_nm gdpp_evt gdpp_dp
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
