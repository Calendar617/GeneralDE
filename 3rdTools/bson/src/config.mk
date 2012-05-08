product:=bson
$(product).type:=lib
$(product).product.c.includes:=3rdTools/bson/include
$(product).c.sources:=$(addprefix $(product-base)/, \
                           bson.c \
                           encoding.c \
                           numbers.c \
                           )
$(product).c.flags.cpp:=-D_POSIX_SOURCE -DMONGO_HAVE_STDINT
$(product).c.flags.c:= -O3
$(product).c.flags.ld:=
$(product).product.c.defs:=MONGO_HAVE_STDINT

$(eval $(call product-def,$(product)))
