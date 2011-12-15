product:=testenv.gd_dp
$(product).type:=lib
$(product).buildfor:=dev
$(product).depends:=testenv.utils gd_dp
$(product).c.sources:=$(wildcard $(product-base)*.cpp)

$(eval $(call product-def,$(product)))
