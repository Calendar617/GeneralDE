product:=cpe_dr_meta_inout
$(product).type:=lib
$(product).depends:=cpe_dr xml2 cpe_utils
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
