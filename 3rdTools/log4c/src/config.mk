product:=log4c
$(product).type:=clib
$(product).sources:= $(addprefix $(product-base), \
                         rc.c \
                         init.c \
                         appender_type_stream.c \
                         appender_type_stream2.c \
                         appender_type_syslog.c \
                         appender_type_mmap.c \
                         layout_type_basic.c \
                         layout_type_dated.c \
                         layout_type_basic_r.c \
                         layout_type_dated_r.c \
                         version.c \
                         logging_event.c \
                         priority.c \
                         appender.c \
                         layout.c \
                         category.c \
                         appender_type_rollingfile.c \
                         rollingpolicy.c \
                         rollingpolicy_type_sizewin.c) \
                    $(addprefix $(product-base)sd/, \
                         stack.c \
                         list.c \
                         malloc.c \
                         factory.c \
                         hash.c \
                         sprintf.c \
                         test.c \
                         sd_xplatform.c \
                         error.c \
                         domnode.c \
                         domnode-xml.c \
                         domnode-xml-parser.c \
                         domnode-xml-scanner.c )

$(product).product.include:=3rdTools/$(product)/include
$(product).flags.ld:=
$(product).flags.cpp:=-DHAVE_CONFIG_H -I$(product-base)$(OS_NAME) -I$(product-base) -DLOG4C_RCPATH=\"\"

$(eval $(call product-def,$(product)))
