/*
**  @file $RCSfile: tuio.h,v $
**  general description of this module
**  $Id: tuio.h,v 1.1.1.1 2008-05-28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008-05-28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TUIO_H

#include "cpe/pal/types.h"

#define TUIO_H

#ifdef WIN32

struct iovec {
	void *iov_base;   /* Starting address */
	size_t iov_len;   /* Number of bytes */
};

ssize_t readv(int fd, const struct iovec *vector, int count);
ssize_t writev(int fd, const struct iovec *vector, int count);

#else /* WIN32 */
#endif /* WIN32	*/

#endif /* TUIO_H */
