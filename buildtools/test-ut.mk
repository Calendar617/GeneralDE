ifneq ($(CPDE_ROOT_INCLEDED),1)
$(error "include repository.mk first!"
endif

$(call assert-not-null,target-product)

include $(CPED_BUILD_DETAIL_DIR)/create-dirs.mk

.PHONY: ut

all: ut

ut: $(foreach domain,$(sort $(domain-list)) \
        , $(if $(filter $(target-product),$($(domain).product-list)) \
            , $(domain).$(target-product).run))

clean: $(target-product).clean
