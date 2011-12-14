product-support-types+=install-dirs
product-def-all-items+=source-dirs target-dirs source-r-dirs target-r-dirs

# $(call install-def-rule-copy,product-name,source,target)
define install-def-rule-copy

$1: $3

$3: $2
	$(call with_message,install $2 to $3 ...)cp $$< $$@

$(eval r.$1.cleanup += $3)

endef

# $(call install-def-rule-copy,product-name,source-dir,target-dir)
define install-def-rule-one-dir-r
auto-build-dirs+=$(CPDE_OUTPUT_ROOT)/$3 \
                 $(subst $(CPDE_ROOT)/$2,$(CPDE_OUTPUT_ROOT)/$3,$(sort $(dir $(shell find $(CPDE_ROOT)/$2 -type f))))

$(foreach f,$(shell find $(CPDE_ROOT)/$2 -type f),\
    $(eval $(call install-def-rule-copy,$1,$f,$(subst $(CPDE_ROOT)/$2,$(CPDE_OUTPUT_ROOT)/$3,$f))) \
)
endef

# $(call install-def-rule-copy,product-name,source-dir,target-dir)
define install-def-rule-one-dir
auto-build-dirs+=$(CPDE_OUTPUT_ROOT)/$3

$(foreach f,$(shell find $(CPDE_ROOT)/$2 -maxdepth 1 -type f),\
    $(eval $(call install-def-rule-copy,$1,$f,$(subst $(CPDE_ROOT)/$2,$(CPDE_OUTPUT_ROOT)/$3,$f))) \
)
endef

define install-def-rule-all-dirs
$(if $2,$(call install-def-rule-one-dir,$1,$(firstword $2),$(firstword $3))\
          $(call install-def-rule-all-dirs,$1,$(wordlist 2,$(words $2), $2),$(wordlist 2,$(words $3), $3)),)
endef

define install-def-rule-all-dirs-r
$(if $2,$(call install-def-rule-one-dir-r,$1,$(firstword $2),$(firstword $3))\
          $(call install-def-rule-all-dirs-r,$1,$(wordlist 2,$(words $2), $2),$(wordlist 2,$(words $3), $3)),)
endef

define product-def-rule-install-dirs

$(if $(filter $(words $(r.$1.source-dirs)),$(words $(r.$1.target-dirs))),,\
    $(error count error! r.$1.source-dirs=$(r.$1.source-dirs) <--> r.$1.target-dirs=$(r.$1.target-dirs)))

$(call install-def-rule-all-dirs,$1,$(r.$1.source-dirs),$(r.$1.target-dirs))
$(call install-def-rule-all-dirs-r,$1,$(r.$1.source-r-dirs),$(r.$1.target-r-dirs))

endef
