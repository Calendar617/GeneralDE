product:=cpe_dr_cvt
$(product).type:=lib 
$(product).depends:=cpe_dr
$(product).c.libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(eval $(call product-def,$(product)))
