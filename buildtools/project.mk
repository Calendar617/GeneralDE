ifneq ($(CPDE_ROOT_INCLEDED),1)
$(error "include repository.mk first!"
endif

product-list?=$(filter-out example%,$(project_repository))

all: $(product-list)

clean: clean-gtags
	$(call with_message,cleaning ...)$(RM) -r $(CPDE_OUTPUT_ROOT)

.PHONY: ut

ut: $(foreach domain,$(sort $(domain-list)) \
         , $(if $(filter 0,$($(domain).ut)) \
                , \
                , $(foreach product,$($(domain).product-list) \
                            , $(if $($(product).ut),$($(product).ut).$(domain).run))))

