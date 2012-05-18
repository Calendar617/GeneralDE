#include "inet_sch_internal_ops.h"

const char * inet_sch_curl_code_msg(CURLMcode code) {
    switch (code) {
    case CURLM_OK: return "CURLM_OK";
    case CURLM_CALL_MULTI_PERFORM: return "CURLM_CALL_MULTI_PERFORM";
    case CURLM_BAD_HANDLE:         return "CURLM_BAD_HANDLE";
    case CURLM_BAD_EASY_HANDLE:    return "CURLM_BAD_EASY_HANDLE";
    case CURLM_OUT_OF_MEMORY:      return "CURLM_OUT_OF_MEMORY";
    case CURLM_INTERNAL_ERROR:     return "CURLM_INTERNAL_ERROR";
    case CURLM_UNKNOWN_OPTION:     return "CURLM_UNKNOWN_OPTION";
    case CURLM_LAST:               return "CURLM_LAST";
    case CURLM_BAD_SOCKET:         return "CURLM_BAD_SOCKET";
    default: return "CURLM_unknown";
    }
}

