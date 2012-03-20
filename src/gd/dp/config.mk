product:=gd_dp
$(product).type:=lib
$(product).depends:=cpe_utils cpe_cfg gd_nm
$(product).libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
