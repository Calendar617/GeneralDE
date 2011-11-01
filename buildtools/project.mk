ifneq ($(CPDE_ROOT_INCLEDED),1)
$(error "include repository.mk first!"
endif

all: $(foreach product,$(project_repository),$(product))

clean: clean-gtags
	$(call with_message,cleaning ...)$(RM) -r $(CPDE_OUTPUT_ROOT)

.PHONY: ut

ut: $(foreach product,$(project_repository),$(if $($(product).ut),$($(product).ut).run))

