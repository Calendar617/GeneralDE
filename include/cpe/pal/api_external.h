#ifndef CPE_PAL_EXTERNAL_H
#define CPE_PAL_EXTERNAL_H

#if defined(WIN32) && !defined(CPE_BUILD_STATIC)
	#define CPE_API_EXPORT __declspec(dllexport)
	#define CPE_API_IMPORT extern __declspec(dllimport)
#else
	#define CPE_API_EXPORT
	#define CPE_API_IMPORT
#endif

#endif


