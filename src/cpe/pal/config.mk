product:=cpe_pal
cpe_pal.type:=clib
cpe_pal.libraries:=
cpe_pal.sources:=$(wildcard $(product-base)*.c)

$(eval $(call product-def,$(product)))
