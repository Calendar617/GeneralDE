#ifndef GD_OM_ERROR_H
#define GD_OM_ERROR_H
#include "om_types.h"

enum gd_om_error {
    gd_om_success = 0
    , gd_om_no_memory
    , gd_om_error_no_class
    , gd_om_error_invalid_oid
    , gd_om_page_head_error
};

#endif
