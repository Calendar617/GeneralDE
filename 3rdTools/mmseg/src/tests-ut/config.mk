product:=gtest.libmmseg
$(product).type:=progn run
$(product).buildfor:=dev
$(product).depends:=gtest libmmseg
$(product).c.sources := $(wildcard $(product-base)/*.cpp)
$(product).c.flags.ld:=$$(r.libmmseg.c.flags.ld)
$(product).c.includes=$$(r.libmmseg.c.includes)
$(product).run.path:=$(product-base)/../..

libmmseg.ut:=$(product)

$(eval $(call product-def,$(product)))
