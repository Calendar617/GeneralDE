ifneq ($(CPDE_ROOT_INCLEDED),1)
$(error "include repository.mk first!"
endif

$(call assert-not-null,target-product)

all: $(target-product)

ut: $(if $($(target-product).ut) \
         , $(foreach domain,$(sort $(domain-list)) \
             , $(if $(filter 0,$($(domain).ut)) \
                   , \
                   , $($(target-product).ut).$(domain).run)))

all: ut

clean: $(target-product).clean

.PHONY: clean-all
clean-all:
	$(call with_message,"clean all...")$(RM) -r $(CPDE_OUTPUT_ROOT)