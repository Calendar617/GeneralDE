product:=gd_timer
$(product).type:=lib
$(product).depends:=cpe_tl gd_app
$(product).c.libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
