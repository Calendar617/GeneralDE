
define domain-target-def
.PHONY: $1

$1: $($1.product-list)

endef

define domain-auto-def-product
$(foreach p,$(call product-gen-depend-list,$($1.product-list)), \
	$(if $(filter $p,$($1.product-list)),,\
		$(eval $(call product-def-for-domain,$p,$1))))
endef

$(foreach domain,$(sort $(domain-list)),\
    $(eval $(call domain-auto-def-product,$(domain))) \
	$(eval $(call domain-target-def,$(domain)))\
)

$(if $(filter-out tools,$(domain-list))\
    , \
    , $(foreach p,$(project_repository),\
          $(eval $(call product-def-for-domain,$p,default))))

$(foreach env,$(sort $(foreach domain,$(sort $(domain-list)),$($(domain).env))), \
    $(call $(env).check))