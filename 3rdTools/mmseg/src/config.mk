product:=mmseg
$(product).depends:=
$(product).type:=lib
$(product).product.c.includes:=3rdTools/mmseg/include
$(product).c.sources:=$(addprefix $(product-base)/, \
                                  css/mmthunk.cpp \
                                  css/segmenter.cpp \
                                  css/SegmenterManager.cpp \
                                  css/SegmentPkg.cpp \
                                  css/SynonymsDict.cpp \
                                  css/ThesaurusDict.cpp \
                                  css/UnigramCorpusReader.cpp \
                                  css/UnigramDict.cpp \
                                  css/UnigramRecord.cpp \
                                  iniparser/dictionary.c \
                                  iniparser/iniparser.c \
                                  utils/assert.c \
                                  utils/bsd_getopt.c \
                                  utils/bsd_getopt_win.c \
                                  utils/csr_mmap.c \
                                  utils/csr_utils.c \
                                  utils/StringTokenizer.cpp \
                                  utils/Utf8_16.cpp \
                           )
$(product).c.includes:=3rdTools/mmseg/src \
                       3rdTools/mmseg/src/utils \
                       3rdTools/mmseg/src/css \
                       3rdTools/mmseg/src/iniparser 
$(product).c.env-includes:=3rdTools/mmseg/src
$(product).c.flags.cpp:=
$(product).c.flags.lan.c:= -O3 -Wno-unused -Wno-sign-compare
$(product).c.flags.lan.cc:= -O3 -Wno-unused -Wno-sign-compare
$(product).c.flags.ld:=
$(product).version:=3.2.14

$(eval $(call product-def,$(product)))
