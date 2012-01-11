#if defined _MSC_VER
#include <string.h>
# define bzero(p,l) memset((p),0,(l))
#else
#include <strings.h>
#endif
