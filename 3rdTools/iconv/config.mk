product:=iconv

$(product).type:=virtual
$(product).product.c.env-libraries+=iconv

$(eval $(call product-def,$(product)))
