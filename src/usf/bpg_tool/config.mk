product:=usf_bpg_tool
$(product).type:=progn 
$(product).depends:=argtable2 \
                    cpe_dr_data_json \
                    gd_app \
                    usf_bpg_net 
$(product).c.libraries:=
$(product).c.sources:=$(wildcard $(product-base)/*.c)
$(product).c.flags.ld:=-rdynamic \
                       $(foreach m,\
                            app_net_runner \
                            dr_store_manage \
                            dr_store_loader \
                            bpg_manage \
                            bpg_net_client \
                            , -u _$m_app_init -u _$m_app_fini)
$(eval $(call product-def,$(product),tools))
