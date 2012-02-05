# {{{ 注册到product-def.mk中
product-support-types+=install
product-def-all-items+=install
# }}}
# {{{ 给拥护使用的辅助函数，用于定义安装的目标
#$(call def-copy-file,src-file,target-file)
def-copy-file=install-def-sep copy-file $1 $2

#$(call def-copy-dir,src-dir,target-dir,postfix-list)
def-copy-dir=install-def-sep copy-dir $1 $2 $3

#$(call def-copy-dir-r,src-dir,target-dir,postfix-list)
def-copy-dir-r=install-def-sep copy-dir-r $1 $2 $3

#$(call def-cvt-file,src-file,target-file,def-file)
def-cvt-file=install-def-sep cvt-file $1 $2 $3
# }}}
# {{{ 实现辅助函数

# $(call install-def-rule-copy,product-name,source,target)
define install-def-rule-copy

$1: $3

$3: $2
	$(call with_message,copy $(subst $(CPDE_ROOT)/,,$2) to $(subst $(CPDE_ROOT)/,,$3) ...)\
         cp $$< $$@

$(eval r.$1.cleanup += $3)

endef

# $(call install-def-rule-cvt,product-name,source,target,cvt-def)
define install-def-rule-cvt

$1: $3

$3: $2 $4
	$(call with_message,convert $(subst $(CPDE_ROOT)/,,$2) to $(subst $(CPDE_ROOT)/,,$3) ...)\
          cp $$< $$@

$(eval r.$1.cleanup += $3)

endef

# $(call install-def-rule-copy,product-name,source-dir,target-dir,postfix-list)
define install-def-rule-one-dir-r
auto-build-dirs+=$(CPDE_OUTPUT_ROOT)/$3 \
                 $(subst $(CPDE_ROOT)/$2,$(CPDE_OUTPUT_ROOT)/$3,$(sort $(dir $(if $(wildcard $(CPDE_ROOT)/$2),$(shell find $(CPDE_ROOT)/$2 -type f)))))

$(foreach f,$(if $(wildcard $(CPDE_ROOT)/$2),\
                 $(shell find $(CPDE_ROOT)/$2 -type f \
                              $(if $4, -o -name "*.$(word 1,$4)" $(foreach p,$(wordlist 2,$(words $4),$4), -o -name "*.$p")))),\
    $(eval $(call install-def-rule-copy,$1,$f,$(subst $(CPDE_ROOT)/$2,$(CPDE_OUTPUT_ROOT)/$3,$f))) \
)
endef

# $(call install-def-rule-one-dir,product-name,source-dir,target-dir,postfix-list)
define install-def-rule-one-dir
auto-build-dirs+=$(CPDE_OUTPUT_ROOT)/$3

$(if $4,\
     $(foreach postfix,$4 \
          , $(foreach f,$(wildcard $(CPDE_ROOT)/$2/*.$(postfix)) \
               , $(call install-def-rule-copy,$1,$f,$(subst $(CPDE_ROOT)/$2,$(CPDE_OUTPUT_ROOT)/$3,$f)))) \
     , $(foreach f,$(if $(wildcard $(CPDE_ROOT)/$2),$(shell find $(CPDE_ROOT)/$2 -maxdepth 1 -type f)) \
           , $(call install-def-rule-copy,$1,$f,$(subst $(CPDE_ROOT)/$2,$(CPDE_OUTPUT_ROOT)/$3,$f))) \
)

endef

# }}}
# {{{ 各种不同类型的安装函数入口,#$1是项目名，$2后续参数各自定义
define product-def-rule-install-copy-file
$(call install-def-rule-copy,\
       $1,\
       $(CPDE_ROOT)/$(word 1,$2),\
       $(CPDE_OUTPUT_ROOT)/$(word 2,$2))
endef

define product-def-rule-install-copy-dir
$(call install-def-rule-one-dir,$1,$(word 1,$2),$(word 2,$2),$(word 3,$2))
endef

define product-def-rule-install-copy-dir-r
$(call install-def-rule-one-dir,$1,$(word 1,$2),$(word 2,$2),$(word 3,$2))
endef

define product-def-rule-install-cvt-file
$(if $(word 3,$2),,$(warning convert input file not set))
$(call assert-file-exists,$(CPDE_ROOT)/$(word 3,$2))

$(call install-def-rule-cvt,\
       $1,\
       $(CPDE_ROOT)/$(word 1,$2),\
       $(CPDE_OUTPUT_ROOT)/$(word 2,$2),\
       $(CPDE_ROOT)/$(word 3,$2))
endef
# }}}
# {{{ 总入口函数
define product-def-rule-install
    $(eval product-def-rule-install-tmp-name:=)
    $(eval product-def-rule-install-tmp-args:=)

	$(foreach w,$(r.$1.install), \
        $(if $(filter install-def-sep,$w) \
            , $(if $(product-def-rule-install-tmp-name) \
                  , $(eval $(call product-def-rule-install-$(product-def-rule-install-tmp-name),$1,$(product-def-rule-install-tmp-args))) \
                    $(eval product-def-rule-install-tmp-name:=) \
                    $(eval product-def-rule-install-tmp-args:=)) \
            , $(if $(product-def-rule-install-tmp-name) \
                  , $(eval product-def-rule-install-tmp-args+=$w) \
                  , $(eval product-def-rule-install-tmp-name:=$w))))

    $(if $(product-def-rule-install-tmp-name) \
        , $(eval $(call product-def-rule-install-$(product-def-rule-install-tmp-name),$1,$(product-def-rule-install-tmp-args))))

endef
# }}}
# {{{ 使用示例
#$(product).install:= $(call def-copy-file,source-file, target-file) \
#                     $(call def-copy-dir-r,source-dir, target-dir, postfix-list) \
#                     $(call def-copy-dir,source-dir, target-dir, postfix-list) \
#                     $(call def-cvt-file,source-file, target-file)
# }}}