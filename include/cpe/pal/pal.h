/*
**  @file $RCSfile: pal.h,v $
**  general description of this module
**  $Id: pal.h,v 1.3 2009-05-27 03:02:30 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-05-27 03:02:30 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef PAL_H
#define PAL_H

/* automatically include the correct library on windows */
#ifdef WIN32

#ifdef TSF4G_SMALL_LIB
#undef _TSF4G_LARGE_LIB_
#endif


#ifdef _TSF4G_LARGE_LIB_
#ifdef _DEBUG
	#pragma comment(lib, "libtsf4g_d.lib")
#else
	#pragma comment(lib, "libtsf4g.lib")
#endif
#else
#ifdef _DEBUG
	#pragma comment(lib, "libpal_d.lib")
#else
	#pragma comment(lib, "libpal.lib")
#endif
#endif

#endif/*#ifdef WIN32*/

#ifndef WIN32
#ifdef _DEBUG
	#define dbgprintf(fmt,args...) printf(__FILE__":%d "fmt"\n",__LINE__,##args)
#else
	#define dbgprintf(fmt,args...) 
#endif
#else
	#define dbgprintf(fmt)
#endif
#include "cpe/pal/api_external.h"
#include "cpe/pal/os.h"
#include "cpe/pal/types.h"
#include "cpe/pal/stdio.h"
#include "cpe/pal/stdlib.h"
#include "cpe/pal/string.h"
#include "cpe/pal/rules.h"
#include "cpe/pal/errno.h"

#include <stddef.h>
#include <assert.h>
#include <stdarg.h>

#include "cpe/pal/uio.h"
#include "cpe/pal/dlfcn.h"
#include "cpe/pal/getopt.h"
#include "cpe/pal/time.h"
#include "cpe/pal/file.h"
#include "cpe/pal/lock.h"
#include "cpe/pal/mutex.h"
#include "cpe/pal/ipc.h"
#include "cpe/pal/shm.h"
#include "cpe/pal/sem.h"
#include "cpe/pal/msgque.h"
#include "cpe/pal/mmap.h"
#include "cpe/pal/socket.h"
#include "cpe/pal/inet.h"
#include "cpe/pal/net.h"
#include "cpe/pal/poll.h"
#include "cpe/pal/fork.h"
#include "cpe/pal/thread.h"
#include "cpe/pal/dirent.h"
#include "cpe/pal/semaphore.h"
#include "cpe/pal/signal.h"
#include "cpe/pal/exec.h"
#include "cpe/pal/wait.h"
#include "cpe/pal/pcreposix.h"

#endif /* PAL_H */
