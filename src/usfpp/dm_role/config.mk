product:=usfpp_dm_role
$(product).type:=lib
$(product).depends:=cpepp_utils gdpp_app usf_dm_role
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
