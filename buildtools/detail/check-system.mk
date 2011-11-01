ORIGN_OS_NAME:=$(shell uname -s)

ifeq ($(ORIGN_OS_NAME),Linux)
OS_NAME:=linux32
endif

ifeq ($(ORIGN_OS_NAME),Darwin)
OS_NAME:=mac
endif

ifeq ($(OS_NAME),)
$(error unknown orign os name $(ORIGN_OS_NAME))
endif
