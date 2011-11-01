product:=curl
curl.type:=clib
curl.libraries:=

curl.sources := $(addprefix $(product-base), \
                     amigaos.c \
                     asyn-ares.c \
                     asyn-thread.c \
                     axtls.c \
                     base64.c \
                     connect.c \
                     content_encoding.c \
                     cookie.c \
                     curl_addrinfo.c \
                     curl_fnmatch.c \
                     curl_gethostname.c \
                     curl_memrchr.c \
                     curl_rand.c \
                     curl_rtmp.c \
                     curl_sspi.c \
                     curl_threads.c \
                     cyassl.c \
                     dict.c \
                     easy.c \
                     escape.c \
                     file.c \
                     fileinfo.c \
                     formdata.c \
                     ftp.c \
                     ftplistparser.c \
                     getenv.c \
                     getinfo.c \
                     gopher.c \
                     gtls.c \
                     hash.c \
                     hmac.c \
                     hostasyn.c \
                     hostip.c \
                     hostip4.c \
                     hostip6.c \
                     hostsyn.c \
                     http.c \
                     http_chunks.c \
                     http_digest.c \
                     http_negotiate.c \
                     http_negotiate_sspi.c \
                     http_ntlm.c \
                     http_proxy.c \
                     idn_win32.c \
                     if2ip.c \
                     imap.c \
                     inet_ntop.c \
                     inet_pton.c \
                     krb4.c \
                     krb5.c \
                     ldap.c \
                     llist.c \
                     md4.c \
                     md5.c \
                     memdebug.c \
                     mprintf.c \
                     multi.c \
                     netrc.c \
                     non-ascii.c \
                     nonblock.c \
                     nss.c \
                     nwlib.c \
                     nwos.c \
                     openldap.c \
                     parsedate.c \
                     pingpong.c \
                     polarssl.c \
                     pop3.c \
                     progress.c \
                     qssl.c \
                     rawstr.c \
                     rtsp.c \
                     security.c \
                     select.c \
                     sendf.c \
                     share.c \
                     slist.c \
                     smtp.c \
                     socks.c \
                     socks_gssapi.c \
                     socks_sspi.c \
                     speedcheck.c \
                     splay.c \
                     ssh.c \
                     sslgen.c \
                     ssluse.c \
                     strdup.c \
                     strequal.c \
                     strerror.c \
                     strtok.c \
                     strtoofft.c \
                     telnet.c \
                     tftp.c \
                     timeval.c \
                     transfer.c \
                     url.c \
                     version.c \
                     warnless.c \
                     wildcard.c)

curl.product.include:=3rdTools/curl/include 3rdTools/curl/include/$(OS_NAME) 3rdTools/curl/include/$(OS_NAME)/curl
curl.flags.cpp:=-DHAVE_CONFIG_H -I$(product-base)$(OS_NAME)
curl.flags.ld:=-lz

$(eval $(call product-def,$(product)))
