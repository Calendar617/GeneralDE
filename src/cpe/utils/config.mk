product:=cpe_utils
$(product).type:=lib
$(product).product.c.includes:=include
$(product).libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
