product:=gtest.gd_tl
$(product).type:=progn
$(product).buildfor:=dev
$(product).depends:=gtest gd_tl cpe_utils
$(product).c.sources := $(wildcard $(product-base)*.cpp)
$(product).run.path:=$(product-base)
gd_tl.ut:=$(product)

$(eval $(call product-def,$(product)))
