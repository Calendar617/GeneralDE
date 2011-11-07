/*
**  @file $RCSfile: tapi_external.h,v $
**   External API definitions
**  $Id ttypes.h,v 1.0 2008/01/10 16:53:32 jackyai Exp $
**  @author jackyai $
**  @date 2008/01/10 16:53:32 $
**  @version 1.0$
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef  TAPI_EXTERNAL_H
#define TAPI_EXTERNAL_H

/** @name ������������/�������
*  @{
*/
/*��ʾ�ò���ֻ���������*/
#ifndef IN
#define IN  
#endif


/*��ʾ�ò���ֻ���������*/
#ifndef OUT
#define OUT
#endif


/*��ʾ�ò���������������������������*/
#ifndef INOUT
#define INOUT
#endif

#ifndef CPE_BUILD_STATIC
	#ifndef CPE_BUILD_SHARED
		#define CPE_BUILD_STATIC
	#endif
#endif



#if defined(WIN32) && !defined(CPE_BUILD_STATIC)
	#define CPE_API_EXPORT __declspec(dllexport)
	#define CPE_API_IMPORT extern __declspec(dllimport)
#else
	#define CPE_API_EXPORT
	#define CPE_API_IMPORT
#endif  /*#if defined(WIN32) && !defined(CPE_BUILD_STATIC)*/


#endif /*TAPI_EXTERNAL_H*/


