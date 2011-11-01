product:=gtest
gtest.type:=cpplib
gtest.buildfor:=dev
gtest.sources:=$(wildcard $(product-base)*.cc)
gtest.product.include:=3rdTools/gtest/include
gtest.flags.cpp:=-I$(product-base)..
gtest.flags.ld:=-lpthread

$(eval $(call product-def,$(product)))
