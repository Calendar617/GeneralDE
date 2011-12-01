product:=gtest.gd_cfg
$(product).type:=progn
$(product).buildfor:=dev
$(product).depends:=gtest gd_cfg cpe_utils cpe_dr yaml
$(product).c.sources := $(wildcard $(product-base)*.cpp)
$(product).run.path:=$(product-base)
gd_cfg.ut:=$(product)

$(eval $(call product-def,$(product)))
