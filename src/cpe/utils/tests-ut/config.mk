# product:=gtest.cpe_utils
# $(product).type:=progn run
# $(product).buildfor:=dev
# $(product).depends:=gtest loki testenv.utils cpe_utils
# $(product).product.c.includes:=include
# $(product).c.sources := $(wildcard $(product-base)*.cpp)
# $(product).c.flags.ld:=
# $(product).run.path:=$(call c-source-dir-to-binary-dir,$(product-base))
# cpe_utils.ut:=$(product)

$(eval $(call gtest-def,cpe_utils))
