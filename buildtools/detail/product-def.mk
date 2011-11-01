CPDE_OUTPUT_ROOT?=$(CPDE_ROOT)/build

project_repository:=
product-support-types:=
product-def-all-items:=type buildfor depends  run run.path
product-def-not-null-items:=type

product-base = $(dir $(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST)))

# $(call product-def,product-name,input-sources)
define product-def
  #verify must set variables
  $(foreach cn,$(product-def-not-null-items),
    $(if $($1.$(cn)),,$(error $(cn) of $1 not defined)))

  #verify type support
  $(if $(filter $($1.type),$(product-support-types)),,
    $(error $1 use not support type $($1.type), supported types: $(product-support-types)))

  #copy variables
  $(foreach cn,$(product-def-all-items),
    $(eval r.$1.$(cn) := $($1.$(cn))))

  $(eval $(call build-regular-path,r.$1.base,$(product-base)))

  #add product to repository
  project_repository+=$1

  .PHONY: $1 $1.clean

  ifneq ($(r.$1.depends),)
  $1: $(r.$1.depends)
  endif

  $(call product-def-rule-$($1.type),$1)

  $1.clean:
	$(call with_message,cleaning...)$(RM) $(r.$1.cleanup)

  ifneq ($(r.$1.run),)
  .PHONY: $1.run

  $1.run: $1
	$(call with_message,runing $1 ...)$(if $(r.$1.run.path),cd $(r.$1.run.path) &&,)$(r.$1.run) $(if $($1-run-args),$($1-run-args),$(r.$1.run.args))

  endif
endef
