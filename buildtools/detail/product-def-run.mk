product-support-types+=run
product-def-all-items+=run.libraries run.cmd run.path run.arg

define product-def-rule-run

.PHONY: $1.run

$1.run: $1
	$(call with_message,runing $1 ...)\
		$$(if $$(r.$1.run.path),cd $$(r.$1.run.path) &&,) \
		$$(if $$(r.$1.run.libraries),LD_LIBRARY_PATH=$$(call path-list-join,$$(r.$1.run.libraries))$$$$LD_LIBRARY_PATH ,) \
		$$(r.$1.run.cmd) \
		$$(r.$1.run.args)

endef
