/*
**  @file $RCSfile: tinet.h,v $
**  general description of this module
**  $Id: tinet.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TINET_H
#define TINET_H

/*********************************************
#FUNCTION:	This head file definite the general shared memory operation.
#AUTHOR:	Niezhiming
#DATE:		12-11-2002
**********************************************/

#ifdef WIN32
#include "cpe/pal/types.h"
#include "cpe/pal/socket.h"
#endif	/* WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif

/* PS Method Start */

#ifdef WIN32

int inet_aton(const char *cp, struct in_addr *inp);

#define inet_network(cp) -1

/* in_addr_t inet_network(const char *cp); */

in_addr_t inet_lnaof(struct in_addr in);
in_addr_t inet_netof(struct in_addr in);

#else /* WIN32 */
#endif /* WIN32 */

/* PS Method End. */

/* PA Method Start */
/* PA Method End. */

#ifdef __cplusplus
}
#endif


#endif /* TINET_H */

