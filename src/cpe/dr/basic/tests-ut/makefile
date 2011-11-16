target-product:=gtest.cpe_dr
include ../../../../../buildtools/repository.mk
include ../../../../../buildtools/test-ut.mk

.PONEY: gen
gen:
	tdr -B -o input.bin input.xml 2>&1 | iconv -f gb2312 -t utf8
