product:=usf_bpg_rsp
$(product).type:=cpe-dr lib
$(product).depends:=cpe_utils cpe_dr_cvt gd_app usf_logic usf_dr_store usf_bpg_pkg
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(product).cpe-dr.modules:=caary
$(product).cpe-dr.caary.generate:=h c
$(product).cpe-dr.caary.source:=$(product-base)/bpg_rsp_carry_info.xml \
                                $(product-base)/bpg_rsp_addition.xml
$(product).cpe-dr.caary.h.output:=protocol
$(product).cpe-dr.caary.c.output:=protocol/caary_package.c
$(product).cpe-dr.caary.c.arg-name:=g_metalib_caary_package

$(eval $(call product-def,$(product)))



