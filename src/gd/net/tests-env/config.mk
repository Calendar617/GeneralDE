product:=testenv.gd_net
$(product).type:=lib
$(product).buildfor:=dev
$(product).depends:=testenv.utils gd_net
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
