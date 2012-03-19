CPDE_OUTPUT_ROOT?=$(CPDE_ROOT)/build

project_repository:=
product-support-types:=
product-def-all-items:=type buildfor depends output
product-def-not-null-items:=type

product-base = $(patsubst %/,%,$(dir $(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))))

#$(call product-gen-depend-list-expand,cur-list,product-list)
product-gen-depend-list-expand=\
  $(if $(strip $2) \
       , $(if $(filter $(strip $(firstword $2)),$1) \
              , $(call product-gen-depend-list-expand\
                       , $(call merge-force-to-end \
                                , $1 \
                                , $(firstword $2) $(call product-gen-depend-list-expand,,$(firstword $2))) \
                       , $(wordlist 2,$(words $2),$2)) \
              , $(call product-gen-depend-list-expand\
                       , $1 $(firstword $2)\
                       , $(wordlist 2,$(words $2),$2) $(r.$(strip $(firstword $2)).depends))) \
       , $1)

#$(call product-gen-depend-list,product-list)
product-gen-depend-list=$(call regular-list,$(call product-gen-depend-list-expand,,$(foreach p,$1,$(r.$p.depends))))

#$(call product-gen-depend-value-list,product-name,value-name-list)
product-gen-depend-value-list=$(call merge-list,,$(foreach p,$(call product-gen-depend-list,$1),$(foreach v,$2,$(r.$p.$v))))

product-gen-depend-value-list-r=$(call merge-list,,$(foreach p,$(call revert-list,$(call product-gen-depend-list,$1)),$(foreach v,$2,$(r.$p.$v))))

# $(call product-def-for-domain,product-name,domain)
define product-def-for-domain

$(eval domain-list+=$2)
$(eval $2.product-list+=$1)

.PHONY:$2.$1
$1: $2.$1

$(if $(r.$1.depends),$2.$1: $(addprefix $2.,$(r.$1.depends)))

$(foreach type,$(if $($1.$2.type),$($1.$2.type),$($1.type)), $(call product-def-rule-$(type),$1,$2))

endef

# $(call product-def,product-name,domain)
define product-def

$(if $2,$(call assert-not-null,$2.output))
$(if $2,$(eval $2.env?=$(OS_NAME)))

#verify must set variables
$(foreach cn,$(product-def-not-null-items),\
  $(if $($1.$(cn)),,$(error $(cn) of $1 not defined)))

#verify type support
$(foreach type,$($1.type),\
    $(if $(filter $(type),$(product-support-types)),,\
        $(error $1 use not support type $(type), supported types: $(product-support-types))))

#copy variables
$(foreach cn,$(product-def-all-items),\
    $(eval r.$1.$(cn):=$($1.$(cn))))

$(eval $(call build-regular-path,r.$1.base,$(product-base)))

#add product to repository
project_repository+=$1

.PHONY: $1 $1.clean

$(if $2,$(call product-def-for-domain,$1,$2))

$1.clean:
	$(call with_message,cleaning...)$(RM) $$(r.$1.cleanup)

endef
