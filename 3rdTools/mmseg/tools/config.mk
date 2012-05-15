product:=mmseg
$(product).type:=progn 
$(product).depends:=libmmseg
$(product).c.libraries:=
$(product).c.sources:=$(product-base)/mmseg_main.cpp
$(product).c.flags.ld:=$$(r.libmmseg.c.flags.ld)
$(product).c.flags.lan.c:=$$(r.libmmseg.c.flags.lan.c)
$(product).c.flags.lan.cc:=$$(r.libmmseg.c.flags.lan.cc)
$(product).c.includes=$$(r.libmmseg.c.includes)

TOOLS_MMSEG=$(CPDE_OUTPUT_ROOT)/$(r.mmseg.tools.product)

$(eval $(call product-def,$(product),tools))
