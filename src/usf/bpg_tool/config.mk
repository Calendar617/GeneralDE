product:=usf_bpg_tool
$(product).type:=progn 
$(product).depends:=argtable2 \
                    dr_json \
                    gd_app \
                    usf_bpg_net 
$(product).c.libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).c.flags.ld:=-rdynamic \
                       $(addprefix -u ,\
                            _app_net_runner_app_fini _app_net_runner_app_init \
                        )
$(eval $(call product-def,$(product),tools))
