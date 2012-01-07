#include <stdarg.h>

#if defined _MSC_VER
# define va_copy(t, s) (t) = (s)
#endif
