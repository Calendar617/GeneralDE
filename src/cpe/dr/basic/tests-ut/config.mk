product:=gtest.cpe_dr
$(product).type:=progn
$(product).buildfor:=dev
$(product).depends:=gtest cpe_utils cpe_dr
$(product).run.path:=$(product-base)
$(product).c.sources := $(wildcard $(product-base)*.cpp)
$(product).c.flags.ld:=
cpe_dr.ut:=$(product)

$(eval $(call product-def,$(product)))
