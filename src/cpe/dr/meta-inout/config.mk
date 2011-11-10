product:=cpe_dr_meta_inout
$(product).type:=clib
$(product).depends:=cpe_dr xml2 cpe_utils
$(product).sources:=$(wildcard $(product-base)*.c)

$(eval $(call product-def,$(product)))
