product:=inet_sch
$(product).type:=lib
$(product).depends:=curl ev gd_app
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))
