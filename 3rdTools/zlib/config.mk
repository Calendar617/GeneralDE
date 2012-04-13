product:=zlib

$(product).type:=virtual
$(product).product.c.libraries+=z

$(eval $(call product-def,$(product)))
