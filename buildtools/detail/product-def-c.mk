GCC?=$(shell which gcc)
CC:=$(GCC)

$(call assert-not-null,GCC)

product-support-types+=lib progn
product-def-all-items+= c.libraries c.includes c.sources c.ldpathes c.flags.cpp c.flags.ld c.linker    \
                        product.c.includes product.c.libraries product.c.ldpathes product.c.defs

c-source-dir-to-binary-dir = $(addprefix $(CPDE_OUTPUT_ROOT)/obj,$(subst $(CPDE_ROOT),,$1))

c-source-to-object = $(call c-source-dir-to-binary-dir,\
						$(subst .m,.o,$(filter %.m,$1))\
						$(subst .cpp,.o,$(filter %.cpp,$1))\
						$(subst .cc,.o,$(filter %.cc,$1))\
						$(subst .c,.o,$(filter %.c,$1)))

c-generate-depend-ld-flags=$(addprefix -L$(CPDE_OUTPUT_ROOT)/,\
								 $(sort $(r.$1.c.ldpathes) $(foreach dep,$(r.$1.depends),$(r.$(dep).product.c.ldpathes)))) \
                           $(addprefix -l,$(sort $(r.$1.c.libraries) $(foreach dep,$(r.$1.depends),$(r.$(dep).product.c.libraries)))) \
                           $(r.$1.c.flags.ld)

c-generate-depend-cpp-flags=$(addprefix -I$(CPDE_ROOT)/,\
								 $(sort $(r.$1.c.includes) $(foreach p,$1 $(r.$1.depends),$(r.$(p).product.c.includes)))) \
                           $(addprefix -D,$(sort $(foreach p,$1 $(r.$1.depends),$(r.$p.product.c.defs)))) \
                           $(r.$1.c.flags.cpp)

# $(call c-make-depend,source-file,object-file,depend-file,product-name)
define c-make-depend
	$(GCC) -MM -MF $3 -MP -MT $2 $(CPPFLAGS) $(call c-generate-depend-cpp-flags,$4) $(TARGET_ARCH) $1
endef

# $(call compile-rule, binary-file, source-files, product-name)
define compile-rule.c

$1: $2
	$$(call with_message)$$(call c-make-depend,$2,$1,$(subst .o,.d,$1),$3)
	$$(call with_message,compiling $(subst $(CPDE_ROOT)/,,$2) --> $(subst $(CPDE_ROOT)/,,$1) ...)\
          $$(COMPILE$(suffix $2)) $$(call c-generate-depend-cpp-flags,$3) -o $$@ $$<

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
ifeq ($(r.$1.run),)
$(eval r.$1.run:=LD_LIBRARY_PATH=$(if $(r.$1.buildfor),$(CPDE_OUTPUT_ROOT)/$(r.$1.buildfor)-lib:,)$(CPDE_OUTPUT_ROOT)/lib:$$$$$$$$LD_LIBRARY_PATH $(CPDE_OUTPUT_ROOT)/$(r.$1.product))
endif
endif

endef

define product-def-rule-lib
$(call product-def-rule-c-product,$1,lib)
endef

define product-def-rule-progn
$(call product-def-rule-c-product,$1,progn)
endef
