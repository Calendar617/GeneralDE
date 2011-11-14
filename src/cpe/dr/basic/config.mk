product:=cpe_dr
$(product).type:=clib
$(product).depends:=cpe_utils
$(product).libraries:=
$(product).sources:=$(wildcard $(product-base)*.c)

$(eval $(call product-def,$(product)))
