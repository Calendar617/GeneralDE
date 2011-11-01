ifneq ($(CPDE_ROOT_INCLEDED),1)
$(error "include repository.mk first!"
endif

$(call assert-not-null,target-product)

all: $(target-product) $(if $($(target-product).ut),$($(target-product).ut).run)

clean: $(target-product).clean

.PHONY: clean-all
clean-all:
	$(call with_message,"clean all...")$(RM) -r $(CPDE_OUTPUT_ROOT)