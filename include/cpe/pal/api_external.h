#ifndef CPE_PAL_EXTERNAL_H
#define CPE_PAL_EXTERNAL_H

#ifdef _MSC_VER
#  define IMPORT_DIRECTIVE __declspec(dllimport) 
#  define EXPORT_DIRECTIVE __declspec(dllexport)
#  define CALL __stdcall
#else
#  define IMPORT_DIRECTIVE __attribute__((__visibility__("default")))
#  define EXPORT_DIRECTIVE __attribute__ ((visibility("default")))
#  define CALL  
#endif

#endif


