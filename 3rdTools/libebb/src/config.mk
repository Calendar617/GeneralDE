product:=ebb
$(product).type:=lib
$(product).depends:=ev
$(product).c.libraries:=
$(product).version:=4.0.4
$(product).c.sources := $(addprefix $(product-base)/, \
                          ebb.c \
                          rbtree.c \
                       )

$(product).product.c.includes:=3rdTools/libebb/include
$(product).c.flags.cpp:=
$(product).c.flags.ld:=

$(eval $(call product-def,$(product)))
