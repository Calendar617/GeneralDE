product:=cpe_dr
$(product).type:=lib
$(product).depends:=cpe_utils
$(product).c.libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).c.flags.cpp:=-Wno-format $(if $(filter 64,$(cpe-dr-metalib)),-D CPE_DR_METALIB_SIZE=64)

$(eval $(call product-def,$(product)))
