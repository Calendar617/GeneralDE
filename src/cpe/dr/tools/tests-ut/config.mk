gtest.cpe_dr_tool_lib.c.flags.cpp:=$(if $(filter 64,$(cpe-dr-metalib)),-D CPE_DR_METALIB_SIZE=64)
$(eval $(call gtest-def,cpe_dr_tool_lib,\
	cpe_dr_meta_inout\
	))
