product:=mongoc
$(product).depends:=bson
$(product).type:=lib
$(product).product.c.includes:=3rdTools/mongoc/include
$(product).c.sources:=$(addprefix $(product-base)/, \
                           env_posix.c \
                           gridfs.c \
                           md5.c \
                           mongo.c \
                           )
$(product).c.flags.cpp:=-D_POSIX_SOURCE
$(product).c.flags.c:= -O3
$(product).c.flags.ld:=

$(eval $(call product-def,$(product)))
