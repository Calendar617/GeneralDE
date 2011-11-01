#include "../dr_internal_types.h"

struct tagDRCTypeInfo g_dr_ctypeinfos[CPE_DR_TYPE_MAX + 1] = {
    {"union", -1}
    , {"struct", -1}
    , {"char", 1}
    , {"uchar", 1}
    , {"byte", 1}
    , {"smallint", 2}
    , {"short", 2}
    , {"smalluint", 2}
    , {"ushort", 2}
    , {"int", 4}
    , {"uint", 4}
    , {"long", 4}
    , {"ulong", 4}
    , {"longlong", 4}
    , {"ulonglong", 4}
    , {"date", 4}
    , {"time", 4}
    , {"datetime", 4}
    , {"money", 4}
    , {"float", 4}
    , {"double", 4}
    , {"ip", 4}
    , {"wchar", 4}
};
