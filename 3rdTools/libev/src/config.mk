product:=ev
$(product).type:=clib
$(product).libraries:=
$(product).version:=4.0.4
$(product).sources := $(addprefix $(product-base), \
                          ev.c \
                          event.c \
                       )

$(product).product.include:=3rdTools/libev/include
$(product).flags.cpp:=-DHAVE_CONFIG_H -I$(product-base)$(dev-env) -Wno-unused -Wno-parentheses
$(product).flags.ld:=

$(eval $(call product-def,$(product)))
