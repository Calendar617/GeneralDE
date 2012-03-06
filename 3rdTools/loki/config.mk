product:=loki
$(product).type:=virtual
$(product).version:=0.1.7
$(product).product.c.includes:=3rdTools/loki/include
$(eval $(call product-def,$(product)))
