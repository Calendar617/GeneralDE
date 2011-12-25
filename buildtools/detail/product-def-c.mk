$(call assert-not-null,GCC)

product-support-types+=lib progn
product-def-all-items+= c.libraries c.includes c.sources c.ldpathes c.flags.cpp c.flags.ld c.linker    \
                        c.flags.lan c.flags.lan.c c.flags.lan.cc c.flags.lan.m c.flags.lan.mm \
                        product.c.includes product.c.libraries product.c.flags.ld product.c.ldpathes product.c.defs

c-source-dir-to-binary-dir = $(addprefix $(CPDE_OUTPUT_ROOT)/obj,$(subst $(CPDE_ROOT),,$1))

c-source-to-object = $(call c-source-dir-to-binary-dir,\
						$(subst .mm,.o,$(filter %.mm,$1))\
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

product-def-c-linker-c=$(LINK.c)
product-def-c-linker-cpp=$(LINK.cc)
product-def-c-linker-obj-c=$(LINK.c)
product-def-c-linker-obj-cpp=$(LINK.cc)

# $(call c-make-depend,source-file,object-file,depend-file,product-name)
define c-make-depend
	$(CCACHE) $(GCC) -MM -MF $3 -MP -MT $2 $(CPPFLAGS) $(call c-generate-depend-cpp-flags,$4) $(TARGET_ARCH) $1
endef

product-def-rule-c-compile-cmd.c=$(CC) $(CFLAGS) $(r.$1.c.flags.lan.all) $(r.$1.c.flags.lan.c) $(CPPFLAGS) $(TARGET_ARCH) -c
product-def-rule-c-compile-cmd.cc=$(CXX) $(CXXFLAGS) $(r.$1.c.flags.lan.all) $(r.$1.c.flags.lan.cc) $(CPPFLAGS) $(TARGET_ARCH) -c 
product-def-rule-c-compile-cmd.mm=$(CXX) $(MMFLAGS) $(r.$1.c.flags.lan.all) $(r.$1.c.flags.lan.mm) $(CPPFLAGS) $(TARGET_ARCH) -c 
product-def-rule-c-compile-cmd.m=$(CC) $(MFLAGS) $(r.$1.c.flags.lan.all) $(r.$1.c.flags.lan.m) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

product-def-rule-c-compile-cmd.cpp=$(call product-def-rule-c-compile-cmd.cc,$1)

# $(call compile-rule, binary-file, source-files, product-name)
define product-def-rule-c-compile-rule
$1: $2
	$$(call with_message)$$(call c-make-depend,$2,$1,$(subst .o,.d,$1),$3)
	$$(call with_message,compiling $(subst $(CPDE_ROOT)/,,$2) --> $(subst $(CPDE_ROOT)/,,$1) ...)\
          $(CCACHE) $$(call product-def-rule-c-compile-cmd$(suffix $2),$3) $$(call c-generate-depend-cpp-flags,$3) -o $$@ $$<

endef

define product-def-c-select-linker
$(strip \
    $(if $(filter .cc,$1),cpp \
	    , $(if $(filter .cpp,$1),cpp \
	        , $(if $(filter .mm,$1),obj-cpp \
	            , $(if $(filter .m,$1),obj-c \
                    , c)))))
endef

define product-def-rule-c-product-for-lib
$(eval r.$1.c.lib.type=$(if $(r.$1.c.lib.type),$(r.$1.c.lib.type),$($(dev-env).default-lib-type)))
r.$1.product.c.libraries+=$1
r.$1.product.c.ldpathes+=$(if $(r.$1.buildfor),$(r.$1.buildfor)-lib,lib)
r.$1.product:=$(if $(r.$1.product),$(r.$1.product),\
                   $(r.$1.output)/lib$1.$(if $(filter static,$(r.$1.c.lib.type)),a,so))
endef

define product-def-rule-c-product-for-progn
r.$1.product:=$(if $(r.$1.product),$(r.$1.product),$(r.$1.output)/$1)
endef

product-def-rule-c-link-cmd-progn=$$(product-def-c-linker-$(r.$1.c.linker)) $2 -o $3 $$(call c-generate-depend-ld-flags,$1)
product-def-rule-c-link-cmd-lib-dynamic=$$(product-def-c-linker-$(r.$1.c.linker)) $(LDFLAGS.share) $2 -o $3 $$(call c-generate-depend-ld-flags,$1)
product-def-rule-c-link-cmd-lib-static=$$(AR) $$(ARFLAGS) $3 $2

# $(call product-def-rule-c-product,product-name,type)
define product-def-rule-c-product

$(eval r.$1.output:=$(if $(r.$1.output),$(r.$1.output),\
                         $(if $(filter lib,$2),\
                              $(if $(r.$1.buildfor),$(r.$1.buildfor)-lib,lib),\
                              $(if $(r.$1.buildfor),$(r.$1.buildfor)-bin,bin))))

$(eval $(if $(filter lib,$2), $(call product-def-rule-c-product-for-lib,$1,$2) \
	      , $(if $(filter progn,$2),$(call product-def-rule-c-product-for-progn,$1,$2)\
                 , $(warning unknown c-product-type of $1: $2)))) \

$(eval r.$1.cleanup+=$(call c-source-to-object,$(r.$1.c.sources)) \
                     $(patsubst %.o,%.d,$(call c-source-to-object,$(r.$1.c.sources))) \
                     $(CPDE_OUTPUT_ROOT)/$(r.$1.product) \
)

$(eval r.$1.c.linker:=$(call product-def-c-select-linker, $(suffix $(r.$1.c.sources))))

$(call assert-not-null,r.$1.c.sources)

auto-build-dirs += $(call c-source-dir-to-binary-dir,$(sort $(dir $(r.$1.c.sources))) $(r.$1.base),./)
auto-build-dirs += $(dir $(CPDE_OUTPUT_ROOT)/$(r.$1.product))

$1: $(CPDE_OUTPUT_ROOT)/$(r.$1.product)

$(CPDE_OUTPUT_ROOT)/$(r.$1.product): $(call c-source-to-object,$(r.$1.c.sources))
	$$(call with_message,linking $(r.$1.product) ...) \
        $(call product-def-rule-c-link-cmd-$(if $(filter progn,$2),progn,lib-$(r.$1.c.lib.type)),$1, $$^, $$@)

$(foreach f,$(r.$1.c.sources),$(call product-def-rule-c-compile-rule,$(call c-source-to-object,$f),$f,$1))

$(eval r.$1.makefile.include := $(patsubst %.o,%.d,$(call c-source-to-object,$(r.$1.c.sources))))

ifeq ($(filter progn,$2),progn)
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
