#ifndef CPEPP_UTILS_CSTRING_H
#define CPEPP_UTILS_CSTRING_H
#include <string.h>
#include "ClassCategory.hpp"

namespace Cpe { namespace Utils {

class CString : public SimulateObject {
public:
    operator const char * (void) const { return (const char *)this; }
    operator const * (void) { return (char *)this; }
    bool isValid(void) const { return this != NULL; }

    bool operator==(CString const & o) const { return strcmp(*this, o) == 0; }
    bool operator!=(CString const & o) const { return strcmp(*this, o) != 0; }
    bool operator<(CString const & o) const { return strcmp(*this, o) < 0; }
    bool operator<=(CString const & o) const { return strcmp(*this, o) <= 0; }
    bool operator>(CString const & o) const { return strcmp(*this, o) > 0; }
    bool operator>=(CString const & o) const { return strcmp(*this, o) >= 0; }

};

}}

#endif
