product:=gtest.cpe_cfg
$(product).type:=progn
$(product).buildfor:=dev
$(product).depends:=gtest loki testenv.utils cpe_cfg cpe_utils cpe_dr yaml
$(product).c.sources := $(wildcard $(product-base)*.cpp)
$(product).run.path:=$(product-base)
cpe_cfg.ut:=$(product)

$(eval $(call product-def,$(product)))
