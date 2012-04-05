product:=xml2
$(product).type:=lib
$(product).depends:=iconv
$(product).version:=2.7.8
$(product).product.c.includes:=3rdTools/libxml2/include

$(product).c.sources := $(addprefix $(product-base)/, \
                   SAX.c \
                   entities.c \
                   encoding.c \
                   error.c \
                   parserInternals.c  \
                   parser.c \
                   tree.c \
                   hash.c \
                   list.c \
                   xmlIO.c \
                   xmlmemory.c \
                   uri.c  \
                   valid.c \
                   xlink.c \
                   HTMLparser.c \
                   HTMLtree.c \
                   debugXML.c \
                   xpath.c   \
                   xpointer.c \
                   xinclude.c \
                   nanohttp.c \
                   nanoftp.c \
                   DOCBparser.c  \
                   catalog.c \
                   globals.c \
                   threads.c \
                   c14n.c \
                   xmlstring.c  \
                   xmlregexp.c \
                   xmlschemas.c \
                   xmlschemastypes.c \
                   xmlunicode.c  \
                   xmlreader.c \
                   relaxng.c \
                   dict.c \
                   SAX2.c  \
                   xmlwriter.c \
                   legacy.c \
                   chvalid.c \
                   pattern.c \
                   xmlsave.c  \
                   xmlmodule.c \
                   schematron.c \
                 )

$(product).product.c.libraries:=z m
$(product).c.libraries:=dl
$(product).c.env-includes:=3rdTools/libxml2/src
$(product).c.flags.cpp:= -DHAVE_CONFIG_H -D_REENTRANT -Wno-format -Wno-unused
$(product).c.flags.ld:=

$(eval $(call product-def,$(product)))
