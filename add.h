//
// MATLAB Compiler: 5.1 (R2014a)
// Date: Mon Jul 03 11:23:33 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:add" "-T" "link:lib" "-d"
// "C:\Users\leiyang\Desktop\add\add\for_testing" "-v"
// "C:\Users\leiyang\Desktop\add\add.m"
// "class{Class1:C:\Users\leiyang\Desktop\add\add.m}" 
//

#ifndef __add_h
#define __add_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_add
#define PUBLIC_add_C_API __global
#else
#define PUBLIC_add_C_API /* No import statement needed. */
#endif

#define LIB_add_C_API PUBLIC_add_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_add
#define PUBLIC_add_C_API __declspec(dllexport)
#else
#define PUBLIC_add_C_API __declspec(dllimport)
#endif

#define LIB_add_C_API PUBLIC_add_C_API


#else

#define LIB_add_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_add_C_API 
#define LIB_add_C_API /* No special import/export declaration */
#endif

extern LIB_add_C_API 
bool MW_CALL_CONV addInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_add_C_API 
bool MW_CALL_CONV addInitialize(void);

extern LIB_add_C_API 
void MW_CALL_CONV addTerminate(void);



extern LIB_add_C_API 
void MW_CALL_CONV addPrintStackTrace(void);

extern LIB_add_C_API 
bool MW_CALL_CONV mlxAdd(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_add
#define PUBLIC_add_CPP_API __declspec(dllexport)
#else
#define PUBLIC_add_CPP_API __declspec(dllimport)
#endif

#define LIB_add_CPP_API PUBLIC_add_CPP_API

#else

#if !defined(LIB_add_CPP_API)
#if defined(LIB_add_C_API)
#define LIB_add_CPP_API LIB_add_C_API
#else
#define LIB_add_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_add_CPP_API void MW_CALL_CONV add(int nargout, mwArray& a, mwArray& b, const mwArray& x, const mwArray& y);

#endif
#endif
