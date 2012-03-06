CPDE_OUTPUT_ROOT?=$(CPDE_ROOT)/build

project_repository:=
product-support-types:=
product-def-all-items:=type buildfor depends output
product-def-not-null-items:=type

product-base = $(patsubst %/,%,$(dir $(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))))

#$(call product-gen-depend-list,product-list)
define product-gen-depend-list
$(sort \
    $(foreach p,$1,\
        $(foreach dep,$(r.$p.depends),\
	        $(dep) $(call product-gen-depend-list,$(dep)))))
endef


#$(call product-gen-depend-value-list,product-name,value-name)
define product-gen-depend-value-list
$(foreach dep,$(r.$1.depends),\
	$(r.$(dep).$2) $(call product-gen-depend-value-list,$(dep),$2))
endef

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
