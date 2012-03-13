product:=testenv.usf_bpg
$(product).type:=lib
$(product).buildfor:=dev
$(product).depends:=testenv.usf_logic \
                    usf_bpg
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
