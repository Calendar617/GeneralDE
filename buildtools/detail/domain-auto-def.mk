
define domain-target-def
.PHONY: $1

$1: $($1.product-list)

$1.ut: $(if $(filter 0,$($1.ut)) \
                , \
                , $(foreach product,$($1.product-list) \
                            , $(if $($(product).ut),$1.$($(product).ut).run)))

endef

define domain-auto-def-product
$(foreach p,$(call product-gen-depend-list,$($1.product-list)), \
	$(if $(filter $p,$($1.product-list)) \
        ,\
        , $(eval $(call product-def-for-domain,$p,$1))) \
    )

$(if $(filter 0,$($1.ut)) \
    , \
    , $(foreach p, $(foreach u,$($1.product-list), $($(u).ut)) \
                   $(call product-gen-depend-list,$(foreach u,$($1.product-list), $($(u).ut))) \
        , $(if $(filter $p,$($1.product-list)) \
               , \
               , $(eval $(call product-def-for-domain,$p,$1)))))

endef

$(foreach domain,$(sort $(domain-list)),\
    $(eval $(call domain-auto-def-product,$(domain))) \
	$(eval $(call domain-target-def,$(domain)))\
)

$(if $(filter-out tools,$(domain-list))\
    , \
    , $(foreach p,$(project_repository),\
          $(eval $(call product-def-for-domain,$p,default))) \
      $(eval $(call domain-target-def,default)) \
	)

$(foreach env,$(sort $(foreach domain,$(sort $(domain-list)),$($(domain).env))), \
    $(call $(env).check))