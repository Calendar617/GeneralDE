/*
**  @file $RCSfile: tstring.h,v $
**  general description of this module
**  $Id: tstring.h,v 1.2 2009-01-23 09:34:20 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-01-23 09:34:20 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSTRING_H
#define TSTRING_H

#ifdef WIN32
#pragma warning(disable:4127)
#endif

#include <string.h>
#include <ctype.h>

#ifdef WIN32
	#include <tchar.h>
#else	/* WIN32 */
#endif	/* WIN32 */

#ifdef WIN32

//#if _MSC_VER < 1300
	//#define	strnlen(sz)				_strncnt(sz)
//#endif	/* _MSC_VER */

/*	#define	strnlen(p, sz)			strlen(p) */

	//#define strupr(sz)				_strupr(sz)
	//#define strlwr(sz)				_strlwr(sz)

//#define strcasecmp(s1, s2)		stricmp(s1, s2)
#ifdef WIN32
	#define strncasecmp(s1, s2, n)	_strnicmp(s1, s2, n)
	#define strcasecmp(s1, s2)		_stricmp(s1, s2)
#else
	#define strncasecmp(s1, s2, n)	strnicmp(s1, s2, n)
	#define strcasecmp(s1, s2)		stricmp(s1, s2)
#endif
	//#define bzero(p,n)				memset(p, 0, n)
	void bzero(void *s, size_t n);

#else

	//#define stricmp(s1, s2)			strcasecmp(s1, s2)
	int stricmp(const char *string1, const char *string2);

	//#define strnicmp(s1, s2, n)		strncasecmp(s1, s2, n)
	int strnicmp(const char *string1, const char *string2,size_t count);

	char *strupr(char *str);
	char *strlwr(char *str);

#endif
#ifdef WIN32


	#if  _MSC_VER < 1400  //VC2003
		#define STRNCPY(pszDst, pszSrc, iLen)	 \
		do								      \
		{								      \
			strncpy((pszDst), (pszSrc), (iLen)-1);			      \
			(pszDst)[(iLen)-1] = 0;					      \
		}								      \
		while(0)
	#else
		#define STRNCPY(pszDst, pszSrc, iLen)					      \
		do								      \
		{								      \
			strncpy_s((pszDst), (iLen),(pszSrc), _TRUNCATE);			      \
			(pszDst)[(iLen)-1] = 0;					      \
		}								      \
		while(0)
	#endif	

#else
	#define STRNCPY(pszDst, pszSrc, iLen)					      \
	do								      \
	{								      \
		strncpy((pszDst), (pszSrc), (iLen)-1);			      \
		(pszDst)[(iLen)-1] = 0;					      \
	}								      \
	while(0)
#endif

#define MEMCPY(d, s, size, min)			{			      \
	int i;								      \
	if( (size)<=(min) )							      \
	{								      \
		for(i=0; i<(size); i++)					      \
		{							      \
			(d)[0]	=	(s)[0];				      \
			(d)++; (s)++;					      \
		}							      \
	}								      \
	else								      \
	{								      \
		memcpy(d, s, size);					      \
		(s) +=	(size);						      \
		(d) +=	(size);						      \
	}								      \
}


#endif /* TSTRING_H */
