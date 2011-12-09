GCC?=$(shell which gcc)
CC:=$(GCC)

$(call assert-not-null,GCC)

product-support-types+=lib progn
product-def-all-items+= c.libraries c.includes c.sources c.ldpathes c.flags.cpp c.flags.ld c.linker    \
                        product.c.includes product.c.libraries product.c.flags.ld product.c.ldpathes product.c.defs

c-source-dir-to-binary-dir = $(addprefix $(CPDE_OUTPUT_ROOT)/obj,$(subst $(CPDE_ROOT),,$1))

c-source-to-object = $(call c-source-dir-to-binary-dir,\
						$(subst .m,.o,$(filter %.m,$1))\
						$(subst .cpp,.o,$(filter %.cpp,$1))\
						$(subst .cc,.o,$(filter %.cc,$1))\
						$(subst .c,.o,$(filter %.c,$1)))

c-generate-depend-ld-flags=$(addprefix -L$(CPDE_OUTPUT_ROOT)/,\
								 $(sort  $(r.$1.c.ldpathes) $(call product-gen-depend-value-list,$1,product.c.ldpathes))) \
                           $(addprefix -l,$(sort $(r.$1.c.libraries) $(call product-gen-depend-value-list,$1,product.c.libraries))) \
                           $(call product-gen-depend-value-list,$1,product.c.flags.ld) \
                           $(r.$1.c.flags.ld)

c-generate-depend-cpp-flags=$(addprefix -I$(CPDE_ROOT)/,\
								 $(sort $(r.$1.c.includes) $(r.$1.product.c.includes) \
									$(call product-gen-depend-value-list,$1,product.c.includes))) \
                           $(addprefix -D,$(sort $(call product-gen-depend-value-list,$1,product.c.defs))) \
                           $(r.$1.c.flags.cpp)

# $(call c-make-depend,source-file,object-file,depend-file,product-name)
define c-make-depend
	$(CCACHE) $(GCC) -MM -MF $3 -MP -MT $2 $(CPPFLAGS) $(call c-generate-depend-cpp-flags,$4) $(TARGET_ARCH) $1
endef

# $(call compile-rule, binary-file, source-files, product-name)
define compile-rule.c
$1: $2
	$$(call with_message)$$(call c-make-depend,$2,$1,$(subst .o,.d,$1),$3)
	$$(call with_message,compiling $(subst $(CPDE_ROOT)/,,$2) --> $(subst $(CPDE_ROOT)/,,$1) ...)\
          $(CCACHE) $$(COMPILE$(suffix $2)) $$(call c-generate-depend-cpp-flags,$3) -o $$@ $$<

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

ifeq ($(filter lib,$3),)
$(eval r.$1.product.c.libraries+=$1)
$(eval r.$1.product.c.ldpathes+=$(if $(r.$1.buildfor),$(r.$1.buildfor)-lib,lib))
endif

$(eval r.$1.cleanup:=$(call c-source-to-object,$(r.$1.c.sources)) \
                     $(patsubst %.o,%.d,$(call c-source-to-object,$(r.$1.c.sources))) \
                     $(CPDE_OUTPUT_ROOT)/$(r.$1.product) \
)

$(eval r.$1.c.linker:=$(if $(r.$1.c.linker),$(r.$1.c.linker),\
                           $(if $(filter %.cpp,$(r.$1.c.sources)),$$(LINK.cc),     \
                                $$(LINK.c)))                                       \
)

$(call assert-not-null,r.$1.c.sources)

auto-build-dirs += $(call c-source-dir-to-binary-dir,$(sort $(dir $(r.$1.c.sources))) $(r.$1.base),./)
auto-build-dirs += $(dir $(CPDE_OUTPUT_ROOT)/$(r.$1.product))

$1: $(CPDE_OUTPUT_ROOT)/$(r.$1.product)

$(CPDE_OUTPUT_ROOT)/$(r.$1.product): $(call c-source-to-object,$(r.$1.c.sources))
	$$(call with_message,linking $(r.$1.product) ...) \
		$$(r.$1.c.linker) $(if $(filter lib,$2),$(LDFLAGS.share)) $$^ -o $$@ $$(call c-generate-depend-ld-flags,$1)


$(foreach f,$(r.$1.c.sources),$(call compile-rule$(suffix $f),$(call c-source-to-object,$f),$f,$1))

$(eval r.$1.makefile.include := $(patsubst %.o,%.d,$(call c-source-to-object,$(r.$1.c.sources))))

ifeq ($(filter progn,$3),)
$(eval r.$1.run.libraries+=$(if $(r.$1.buildfor),$(CPDE_OUTPUT_ROOT)/$(r.$1.buildfor)-lib,) $(CPDE_OUTPUT_ROOT)/lib)
$(eval r.$1.run.cmd:=$(CPDE_OUTPUT_ROOT)/$(r.$1.product))
endif

endef

define product-def-rule-lib
$(call product-def-rule-c-product,$1,lib)
endef

define product-def-rule-progn
$(call product-def-rule-c-product,$1,progn)
endef
