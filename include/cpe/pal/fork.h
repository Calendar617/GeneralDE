/*
**  @file $RCSfile: tfork.h,v $
**  general description of this module
**  $Id: tfork.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TFORK_H

#define TFORK_H

#include "cpe/pal/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32

pid_t fork(void);

#endif

#ifdef __cplusplus
}
#endif


#endif /* TFORK_H */
