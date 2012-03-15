product:=usf_bpg_net
$(product).type:=lib
$(product).depends:=cpe_net usf_bpg
$(product).c.flags.ld:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)

$(eval $(call product-def,$(product)))



