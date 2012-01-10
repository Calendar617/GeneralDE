#ifndef GD_OM_ERROR_H
#define GD_OM_ERROR_H
#include "om_types.h"

enum gd_om_error {
    gd_om_success = 0
    , gd_om_no_memory
    , gd_om_no_buffer
    , gd_om_buffer_get_fail
    , gd_om_invalid_oid
    , gd_om_invalid_align
    , gd_om_invalid_address
    , gd_om_page_head_error
    , gd_om_page_size_too_big
    , gd_om_page_size_too_small
    , gd_om_class_not_exist
    , gd_om_class_name_too_long
    , gd_om_class_overflow
    , gd_om_class_name_duplicate
};

#endif
