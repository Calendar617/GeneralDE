product:=iconv

ifeq ($(dev-env),vc)
$(product).type:=install
$(product).product.c.libraries+=iconv
$(product).install:=$(call def-copy-dir,3rdTools/iconv/$(dev-env)/lib,lib)
$(product).product.c.includes:=3rdTools/iconv/$(dev-env)/include
else
$(product).type:=virtual
$(product).product.c.libraries+=iconv
endif

$(eval $(call product-def,$(product)))
