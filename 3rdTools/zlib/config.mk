product:=zlib

$(product).type:=virtual
$(product).product.c.libraries+=zlib
$(product).vc.install:=$(call def-copy-dir,3rdTools/zlib/$(dev-env)/lib,lib)
$(product).vc.type:=install
$(product).vc.product.c.includes:=3rdTools/zlib/$(dev-env)/include

$(eval $(call product-def,$(product)))
