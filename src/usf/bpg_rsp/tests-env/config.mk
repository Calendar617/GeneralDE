product:=testenv.usf_bpg_rsp
$(product).type:=lib
$(product).buildfor:=dev
$(product).depends:=testenv.usf_logic \
                    usf_bpg_rsp
$(product).c.sources:=$(wildcard $(product-base)/*.cpp)

$(eval $(call product-def,$(product)))
