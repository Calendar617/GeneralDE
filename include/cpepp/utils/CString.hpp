#ifndef CPEPP_UTILS_CSTRING_H
#define CPEPP_UTILS_CSTRING_H
#include <string.h>
#include "ClassCategory.hpp"

namespace Cpe { namespace Utils {

class CString : public SimulateObject {
public:
    operator const char * (void) const { return (const char *)this; }
    operator char * (void) { return (char *)this; }
    bool isValid(void) const { return this != NULL; }

    static CString const & _cast(const char * d) { 
        return * reinterpret_cast<CString const *>(d);
    }

    static CString & _cast(char * d) { 
        return * reinterpret_cast<CString *>(d);
    }
};

inline bool operator==(const char * l, CString const & r) { return strcmp(l, r) == 0; }
inline bool operator!=(const char * l, CString const & r) { return strcmp(l, r) != 0; }
inline bool operator<(const char * l, CString const & r) { return strcmp(l, r) < 0; }
inline bool operator<=(const char * l, CString const & r) { return strcmp(l, r) <= 0; }
inline bool operator>(const char * l, CString const & r) { return strcmp(l, r) > 0; }
inline bool operator>=(const char * l, CString const & r) { return strcmp(l, r) >= 0; }

inline bool operator==(CString const & l, const char * r) { return strcmp(l, r) == 0; }
inline bool operator!=(CString const & l, const char * r) { return strcmp(l, r) != 0; }
inline bool operator<(CString const & l, const char * r) { return strcmp(l, r) < 0; }
inline bool operator<=(CString const & l, const char * r) { return strcmp(l, r) <= 0; }
inline bool operator>(CString const & l, const char * r) { return strcmp(l, r) > 0; }
inline bool operator>=(CString const & l, const char * r) { return strcmp(l, r) >= 0; }

inline bool operator==(CString const & l, CString const & r) { return strcmp(l, r) == 0; }
inline bool operator!=(CString const & l, CString const & r) { return strcmp(l, r) != 0; }
inline bool operator<(CString const & l, CString const & r) { return strcmp(l, r) < 0; }
inline bool operator<=(CString const & l, CString const & r) { return strcmp(l, r) <= 0; }
inline bool operator>(CString const & l, CString const & r) { return strcmp(l, r) > 0; }
inline bool operator>=(CString const & l, CString const & r) { return strcmp(l, r) >= 0; }

}}

#endif
