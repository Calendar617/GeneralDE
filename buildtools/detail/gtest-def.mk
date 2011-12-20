#$(call gtest-def,pruduct,depends)
define gtest-def

$(eval product:=gtest.$1)
$(eval $(product).type:=progn run)
$(eval $(product).buildfor:=dev)
$(eval $(product).depends:=testenv.utils $1 $2)
$(eval $(product).product.c.includes:=include)
$(eval $(product).c.sources := $(wildcard $(product-base)/*.cpp)\
                               $(wildcard $(product-base)/*.c))
$(eval $(product).c.flags.ld:=$$(r.$1.c.flags.ld) -rdynamic)
$(eval $(product).run.path:=$(product-base))
$(eval $1.ut:=$(product))

$(eval $(call product-def,$(product)))

endef