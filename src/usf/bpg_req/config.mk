product:=usf_bpg
$(product).type:=cpe-dr lib
$(product).depends:=cpe_utils cpe_dr_cvt gd_app usf_logic usf_dr_store
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(product).cpe-dr.modules:=base
$(product).cpe-dr.base.generate:=h c
$(product).cpe-dr.base.h.output:=protocol/base
$(product).cpe-dr.base.c.output:=protocol/base/base_package.c
$(product).cpe-dr.base.c.arg-name:=g_metalib_base_package

#$(eval $(call product-def,$(product)))



