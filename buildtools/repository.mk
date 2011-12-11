ifndef CPDE_ROOT_INCLEDED
CPDE_ROOT_INCLEDED:=1

.PHONY: all
all:

space=$(null) $(null)

CPDE_MAKEFILE_PRE:=$(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST))
CPDE_ROOT:=$(subst /buildtools/$(notdir $(CPDE_MAKEFILE_PRE)),,$(CURDIR)/$(CPDE_MAKEFILE_PRE))

CPDE_LIB_DIR:=$(CPDE_ROOT)/lib
CPDE_BIN_DIR:=$(CPDE_ROOT)/bin
CPED_BUILD_DETAIL_DIR:=$(CPDE_ROOT)/buildtools/detail

include $(CPED_BUILD_DETAIL_DIR)/check-system.mk
include $(CPED_BUILD_DETAIL_DIR)/utils.mk
include $(CPED_BUILD_DETAIL_DIR)/change-root.mk

-include $(CPDE_ROOT)/dev-env.mk
dev-env?=$(OS_NAME)
include $(CPED_BUILD_DETAIL_DIR)/dev-env-$(dev-env).mk

include $(CPED_BUILD_DETAIL_DIR)/product-def.mk
include $(wildcard $(CPED_BUILD_DETAIL_DIR)/product-def-*.mk)
include $(CPED_BUILD_DETAIL_DIR)/gtest-def.mk
include $(CPED_BUILD_DETAIL_DIR)/gtags.mk

endif

modules := $(subst /config.mk,,$(shell find $(CPDE_ROOT) -name config.mk))
include $(addsuffix /config.mk,$(modules))

-include $(foreach p,$(project_repository), $(r.$p.makefile.include))

include $(CPED_BUILD_DETAIL_DIR)/create-dirs.mk

