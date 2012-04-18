product-support-types+=cpe-dr
product-def-all-items+=cpe-dr.modules

cpe-dr-tool=$(CPDE_OUTPUT_ROOT)/$(tools.output)/bin/cpe_dr_tool

define product-def-rule-cpe-dr-c-module-validate
  .POHEY: $1.$3.cpe-dr.$2.validate

  $(call c-source-to-object,$(r.$1.c.sources),$3): $1.$3.cpe-dr.$2.validate

  $1.$3.cpe-dr.$2.validate:
	$$(call with_message,cpe-dr validate dr lib $2 ...) \
	LD_LIBRARY_PATH=$(CPDE_OUTPUT_ROOT)/$(tools.output)/lib:$$$$LD_LIBRARY_PATH \
	$(cpe-dr-tool) $(addprefix --validate ,$($1.cpe-dr.$2.validate)) \
                   $(addprefix -i ,$(r.$1.$3.cpe-dr.$2.source)) 

endef

define product-def-rule-cpe-dr-c-module-h
  $(call assert-not-null,$1.cpe-dr.$2.h.output)

  $(eval r.$1.$3.cpe-dr.$2.h.output:=$($1.cpe-dr.$2.h.output))
  $(eval r.$1.$3.cpe-dr.$2.h.output-dir:=$(call c-source-dir-to-binary-dir,$(r.$1.base)/$(r.$1.$3.cpe-dr.$2.h.output),$3))
  $(eval r.$1.$3.cpe-dr.$2.generated.h:=\
      $(addprefix $(r.$1.$3.cpe-dr.$2.h.output-dir)/,\
            $(patsubst %.xml,%.h, $(notdir $(r.$1.$3.cpe-dr.$2.source)))))

  auto-build-dirs += $(r.$1.$3.cpe-dr.$2.h.output-dir)

  $(eval r.$1.cleanup += $(r.$1.$3.cpe-dr.$2.generated.h))
  $(eval r.$1.$3.c.includes+=$(subst $(CPDE_ROOT),,$(call c-source-dir-to-binary-dir,$(r.$1.base),$3)))

  $(call c-source-to-object,$(r.$1.c.sources),$3): $(r.$1.$3.cpe-dr.$2.generated.h)

  $(foreach source,$(r.$1.$3.cpe-dr.$2.source),\
      $(r.$1.$3.cpe-dr.$2.h.output-dir)/$(patsubst %.xml,%.h,$(notdir $(source)))): $(source) $(cpe-dr-tool)

  $(r.$1.$3.cpe-dr.$2.generated.h): $(r.$1.$3.cpe-dr.$2.source) 
	$$(call with_message,cpe-dr generaing h to $(subst $(CPDE_ROOT)/,,$(r.$1.$3.cpe-dr.$2.h.output-dir)) ...) \
	LD_LIBRARY_PATH=$(CPDE_OUTPUT_ROOT)/$(tools.output)/lib:$$$$LD_LIBRARY_PATH \
	$(cpe-dr-tool) $(addprefix -i ,$(r.$1.$3.cpe-dr.$2.source)) \
                   --output-h $(r.$1.$3.cpe-dr.$2.h.output-dir)

endef

define product-def-rule-cpe-dr-c-module-c
  $(call assert-not-null,$1.cpe-dr.$2.c.output)

  $(eval r.$1.$3.cpe-dr.$2.c.output:=$($1.cpe-dr.$2.c.output))
  $(eval r.$1.$3.cpe-dr.$2.c.arg-name:=$($1.cpe-dr.$2.c.arg-name))
  $(eval r.$1.$3.cpe-dr.$2.c.output-dir:=$(call c-source-dir-to-binary-dir,$(r.$1.base)/$(patsubst %/,%,$(dir $(r.$1.$3.cpe-dr.$2.c.output))),$3))
  $(eval r.$1.$3.cpe-dr.$2.generated.c:=$(r.$1.$3.cpe-dr.$2.c.output-dir)/$(notdir $($1.cpe-dr.$2.c.output)))

  auto-build-dirs += $(r.$1.$3.cpe-dr.$2.c.output-dir)

  $(eval r.$1.$3.c.sources += $(r.$1.$3.cpe-dr.$2.generated.c))
  $(eval r.$1.cleanup += $(r.$1.$3.cpe-dr.$2.generated.c))

  $(r.$1.$3.cpe-dr.$2.generated.c): $(r.$1.$3.cpe-dr.$2.source) $(cpe-dr-tool)
	$$(call with_message,cpe-dr generaing lib-c to $(subst $(CPDE_ROOT)/,,$(r.$1.$3.cpe-dr.$2.generated.c)) ...) \
	LD_LIBRARY_PATH=$(CPDE_OUTPUT_ROOT)/$(tools.output)/lib:$$$$LD_LIBRARY_PATH \
	$(cpe-dr-tool) $(addprefix -i ,$(r.$1.$3.cpe-dr.$2.source)) \
                   --output-lib-c $$@ --output-lib-c-arg $($1.cpe-dr.$2.c.arg-name)

endef

# $(call product-def-rule-cpe-dr-c-module-bin,$1,$2,domain)
define product-def-rule-cpe-dr-c-module-bin
  $(call assert-not-null,$1.cpe-dr.$2.bin.output)

  $(eval r.$1.$3.cpe-dr.$2.bin.output:=$($1.cpe-dr.$2.bin.output))
  $(eval r.$1.$3.cpe-dr.$2.bin.output-dir:=$(CPDE_OUTPUT_ROOT)/$($3.output)/$(patsubst %/,%,$(dir $(r.$1.$3.cpe-dr.$2.bin.output))))
  $(eval r.$1.$3.cpe-dr.$2.generated.bin:=$(r.$1.$3.cpe-dr.$2.bin.output-dir)/$(notdir $($1.cpe-dr.$2.bin.output)))

  auto-build-dirs += $(r.$1.$3.cpe-dr.$2.bin.output-dir)

  $(eval r.$1.cleanup += $(r.$1.$3.cpe-dr.$2.generated.bin))

  $1: $3.$1

  $3.$1: $(r.$1.$3.cpe-dr.$2.generated.bin)

  $(r.$1.$3.cpe-dr.$2.generated.bin): $(cpe-dr-tool)

  $(r.$1.$3.cpe-dr.$2.generated.bin): $(r.$1.$3.cpe-dr.$2.source) 
	$$(call with_message,cpe-dr generaing bin to $(subst $(CPDE_ROOT)/,,$(r.$1.$3.cpe-dr.$2.generated.bin)) ...) \
	LD_LIBRARY_PATH=$(CPDE_OUTPUT_ROOT)/$(tools.output)/lib:$$$$LD_LIBRARY_PATH \
	$(cpe-dr-tool) $(addprefix -i ,$(r.$1.$3.cpe-dr.$2.source)) \
                   --output-lib-bin $$@ 

endef

define product-def-rule-cpe-dr-c-module
$(call assert-not-null,$1.cpe-dr.$3.source)
$(call assert-not-null,$1.cpe-dr.$3.generate)

$(eval r.$1.$2.cpe-dr.$3.source:=$($1.cpe-dr.$3.source))
$(eval r.$1.$2.cpe-dr.$3.generate:=$($1.cpe-dr.$3.generate))

$(foreach p,$(r.$1.$2.cpe-dr.$3.generate), $(call product-def-rule-cpe-dr-c-module-$p,$1,$3,$2))

endef

define product-def-rule-cpe-dr

$(foreach module,$(r.$1.cpe-dr.modules),\
	$(call product-def-rule-cpe-dr-c-module,$1,$2,$(module)))

endef