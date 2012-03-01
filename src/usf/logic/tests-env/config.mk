product:=testenv.usf_logic
$(product).type:=lib
$(product).buildfor:=dev
$(product).depends:=testenv.gd_app \
                    usf_logic \
                    gmock
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
