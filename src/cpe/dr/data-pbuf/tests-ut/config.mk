gtest.cpe_dr_data_pbuf.c.flags.cpp:=$(if $(filter 64,$(cpe-dr-metalib)),-D CPE_DR_METALIB_SIZE=64)
$(eval $(call gtest-def,cpe_dr_data_pbuf,testenv.cpe_cfg cpe_dr_data_cfg cpe_dr_data_pbuf cpe_dr_meta_inout))
