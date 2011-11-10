product:=cpe_utils
$(product).type:=clib
$(product).libraries:=
$(product).sources:=$(wildcard $(product-base)*.c)

$(eval $(call product-def,$(product)))
