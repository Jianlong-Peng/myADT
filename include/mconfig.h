/*=============================================================================
#     FileName: mconfig.h
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Version: 0.0.1
#      Created: 2014-07-25 14:52:26
#   LastChange: 2014-08-04 12:04:14
#      History: 
=============================================================================*/
#ifndef  MY_CONFIG_H_INC
#define  MY_CONFIG_H_INC

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(FALSE)
typedef int BOOL;
#define FALSE 0
#define TRUE 1
#endif

#if defined(WIN32) && defined(USING_DYNAMIC_LIBS) && !defined(__MINGW32__)
 #define ADT_EXPORT __declspec(dllexport)
 #define ADT_IMPORT __declspec(dllimport)
#else
 #define ADT_EXPORT
 #define ADT_IMPORT
#endif


#if defined(MAKE_DLL)
 #ifndef ADT_API
  #define ADT_API ADT_EXPORT
 #endif
 
#else  // defined(MAKE_DLL)

 #ifndef ADT_API
  #define ADT_API ADT_IMPORT
 #endif
 
#endif

#ifdef __cplusplus
}
#endif

#endif   /* ----- #ifndef MY_CONFIG_H_INC  ----- */

