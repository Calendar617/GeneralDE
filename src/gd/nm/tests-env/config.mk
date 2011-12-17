product:=testenv.gd_nm
$(product).type:=lib
$(product).buildfor:=dev
$(product).depends:=testenv.utils gd_nm
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
