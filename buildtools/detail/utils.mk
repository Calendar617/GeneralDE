# $(call assert,condition,message)
define assert
  $(if $1,,$(error Assertion failed: $2))
endef

# $(call assert-file-exists,wildcad-pattern)
define assert-file-exists
  $(call assert,$(wildcard $1),$1 does not exit)
endef

# $(call assert-not-null,make-variable)
define assert-not-null
  $(call assert,$($1),The variable "$1" is null)
endef

# $(call join-path,part1,part2)
join-path=$(if $(1),$(strip $(1))/$(strip $(2)),$(2))

ifneq ($(MAKE_SILENCE),1)
  define with_message
    @$(if $1,echo ">>> $1" &&, )
  endef
endif

ifeq ($(MAKEFILE_DEBUG),1)
  debug-warning=$(warning $1)
  debug-variables=$(warning $1)
endif

# $(call stack-push,stack-variable-name,word)
define stack-push
$1 := $2 $($1)
endef

# $(call stack-pop,stack-variable-name)
define stack-pop
$1 := $(wordlist 2,$(words $($1)),$($1))
endef

# $(call build-regular-path,output-variable,input-variable)
define build-regular-path
$(foreach part,$(subst /,$(space),$2),                          \
    $(if $(filter .,$(part)),,                                  \
    $(if $(filter ..,$(part)),$(eval $(call stack-pop,$1.inbuild))      \
                             ,$(eval $(call stack-push,$1.inbuild,$(part)))))   \
)
$(foreach result-part,$($1.inbuild),$(eval $1:=/$(result-part)$($1)))
endef

# $(call path-list-join,path-list)
define path-list-join
$(if $1,$(word 1,$1):$(call path-list-join,$(wordlist 2,$(words $1), $1)),)
endef

# $(call select-var,var-list)
define select-var
$(if $1\
     , $(warning $(word 1,$1))\
	   $(if $($(word 1, $1)) \
          , $($(word 1, $1))\
          , $(call select-var,$(wordlist 2,$(words $1), $1))) \
     ,)
endef
