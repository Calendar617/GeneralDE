GCC?=$(shell which gcc)
CC:=$(GCC)

$(call assert-not-null,GCC)

product-support-types+=clib cpplib objclib cprogn cppprogn objcprogn
product-def-all-items+= libraries sources product.include product.lib flags.cpp flags.ld

c-source-dir-to-binary-dir = $(addprefix $(CPDE_OUTPUT_ROOT)/obj,$(subst $(CPDE_ROOT),,$1))

c-source-to-object = $(call c-source-dir-to-binary-dir,\
						$(subst .m,.o,$(filter %.m,$1))\
						$(subst .cpp,.o,$(filter %.cpp,$1))\
						$(subst .cc,.o,$(filter %.cc,$1))\
						$(subst .c,.o,$(filter %.c,$1)))

c-generate-depend-ld-flags=$(foreach dep,$(r.$1.depends),\
                              $(if $(filter $(r.$(dep).type),clib cpplib objclib),\
                                    -L$(dir $(CPDE_OUTPUT_ROOT)/$(r.$(dep).product)) -l$(dep) $(addprefix -l,$(r.$(dep).product.lib)) \
                              ,))

# $(call c-make-depend,source-file,object-file,depend-file)
define c-make-depend
	$(GCC) -MM -MF $3 -MP -MT $2 $(CPPFLAGS) $(TARGET_ARCH) $1
endef

# $(call compile-rule, binary-file, source-files, product-name)
define compile-rule.c

ifneq ($(strip $(r.$3.product.include) $(r.$3.flags.cpp)),)
$1: CPPFLAGS+=$(r.$3.flags.cpp) $(addprefix -I$(CPDE_ROOT)/,$(r.$3.product.include))
endif

$(foreach dep,$(r.$3.depends),\
	    $(if $(r.$(dep).product.include),$(eval $1: CPPFLAGS+=$(addprefix -I$(CPDE_ROOT)/,$(r.$(dep).product.include)))) \
    )

$1: $2
	$$(call with_message)$$(call c-make-depend,$2,$1,$(subst .o,.d,$1))
	$$(call with_message,compiling $(subst $(CPDE_ROOT)/,,$2) --> $(subst $(CPDE_ROOT)/,,$1) ...)$$(COMPILE$(suffix $2)) -o $$@ $$<


endef

define compile-rule.cc
$(call compile-rule.c,$1,$2,$3)
endef

define compile-rule.cpp
$(call compile-rule.cc,$1,$2,$3)
endef

# $(call product-def-rule-c-product,product-name,type)
define product-def-rule-c-product

$(eval r.$1.product:=$(if $(filter lib,$2),\
                       $(if $(r.$1.buildfor),$(r.$1.buildfor)-lib,lib)/lib$1.so,\
                       $(if $(r.$1.buildfor),$(r.$1.buildfor)-bin,bin)/$1) \
)

$(eval r.$1.cleanup:=$(call c-source-to-object,$(r.$1.sources)) \
                     $(CPDE_OUTPUT_ROOT)/$(r.$1.product) \
)

$(call assert-not-null,r.$1.sources)

auto-build-dirs += $(call c-source-dir-to-binary-dir,$(sort $(dir $(r.$1.sources))) $(r.$1.base),./)
auto-build-dirs += $(dir $(CPDE_OUTPUT_ROOT)/$(r.$1.product))

$1: $(CPDE_OUTPUT_ROOT)/$(r.$1.product)

$(CPDE_OUTPUT_ROOT)/$(r.$1.product): $(call c-source-to-object,$(r.$1.sources))
	$$(call with_message,linking $(r.$1.product) ...)$(if $(filter cpp%,$2),$$(LINK.c),$$(LINK.cc)) $$^ $$(LOADLIBES) \
		$(if $(filter lib,$2),$(LDFLAGS.share))  -o $$@ $$(call c-generate-depend-ld-flags,$1) $(r.$1.flags.ld) 


$(foreach f,$(r.$1.sources),$(call compile-rule$(suffix $f),$(call c-source-to-object,$f),$f,$1))

$(eval r.$1.makefile.include := $(patsubst %.o,%.d,$(call c-source-to-object,$(r.$1.sources))))

ifeq ($(filter progn,$3),)
ifeq ($(r.$1.run),)
$(eval r.$1.run:=LD_LIBRARY_PATH=$(if $(r.$1.buildfor),$(CPDE_OUTPUT_ROOT)/$(r.$1.buildfor)-lib:,)$(CPDE_OUTPUT_ROOT)/lib:$$$$$$$$LD_LIBRARY_PATH $(CPDE_OUTPUT_ROOT)/$(r.$1.product))
endif
endif

endef

define product-def-rule-clib
$(call product-def-rule-c-product,$1,lib)
endef

define product-def-rule-cpplib
$(call product-def-rule-clib,$1)
endef

define product-def-rule-objclib
$(call product-def-rule-c-product,$1,lib)
endef

define product-def-rule-cprogn
$(call product-def-rule-c-product,$1,progn)
endef

define product-def-rule-cppprogn
$(call product-def-rule-c-product,$1,progn)
endef

define product-def-rule-objcprogn
$(call product-def-rule-c-product,$1,progn)
endef
