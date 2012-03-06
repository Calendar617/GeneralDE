product:=iconv

$(product).type:=virtual
$(product).product.c.libraries+=iconv
$(product).vc.install:=$(call def-copy-dir,3rdTools/iconv/$(dev-env)/lib,lib)
$(product).vc.type:=install
$(product).vc.product.c.includes:=3rdTools/iconv/$(dev-env)/include

$(eval $(call product-def,$(product)))
