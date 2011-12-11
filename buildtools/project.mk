ifneq ($(CPDE_ROOT_INCLEDED),1)
$(error "include repository.mk first!"
endif

product-list?=$(filter-out example%,$(project_repository))

all: $(product-list)

clean: clean-gtags
	$(call with_message,cleaning ...)$(RM) -r $(CPDE_OUTPUT_ROOT)

.PHONY: ut

ut: $(foreach product,\
        $(sort $(product-list) $(call product-gen-depend-list,$(product-list))),\
        $(if $($(product).ut),$($(product).ut).run))

