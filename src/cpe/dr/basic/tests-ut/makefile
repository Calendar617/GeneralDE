target-product:=gtest.cpe_dr
include ../../../../../buildtools/repository.mk
include ../../../../../buildtools/test-ut.mk


gen-target=input.bin.32

.PONEY: gen
gen:
	$(CPDE_OUTPUT_ROOT)/tools/bin/tdr -B -o input.bin input.xml 2>&1 | iconv -f gb2312 -t utf8
