//
// MATLAB Compiler: 5.1 (R2014a)
// Date: Thu Jul 06 11:21:11 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:linear_Interpolation" "-T"
// "link:lib" "-d"
// "C:\Users\leiyang\Desktop\linear_Interpolation\linear_Interpolation\for_testi
// ng" "-v"
// "C:\Users\leiyang\Desktop\linear_Interpolation\linear_Interpolation.m"
// "C:\Users\leiyang\Desktop\linear_Interpolation\Link.m"
// "C:\Users\leiyang\Desktop\linear_Interpolation\SerialLink.m"
// "class{Class1:C:\Users\leiyang\Desktop\linear_Interpolation\linear_Interpolat
// ion.m,C:\Users\leiyang\Desktop\linear_Interpolation\Link.m,C:\Users\leiyang\D
// esktop\linear_Interpolation\SerialLink.m}" 
//

#ifndef __linear_Interpolation_h
#define __linear_Interpolation_h 1

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

#ifdef EXPORTING_linear_Interpolation
#define PUBLIC_linear_Interpolation_C_API __global
#else
#define PUBLIC_linear_Interpolation_C_API /* No import statement needed. */
#endif

#define LIB_linear_Interpolation_C_API PUBLIC_linear_Interpolation_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_linear_Interpolation
#define PUBLIC_linear_Interpolation_C_API __declspec(dllexport)
#else
#define PUBLIC_linear_Interpolation_C_API __declspec(dllimport)
#endif

#define LIB_linear_Interpolation_C_API PUBLIC_linear_Interpolation_C_API


#else

#define LIB_linear_Interpolation_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_linear_Interpolation_C_API 
#define LIB_linear_Interpolation_C_API /* No special import/export declaration */
#endif

extern LIB_linear_Interpolation_C_API 
bool MW_CALL_CONV linear_InterpolationInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_linear_Interpolation_C_API 
bool MW_CALL_CONV linear_InterpolationInitialize(void);

extern LIB_linear_Interpolation_C_API 
void MW_CALL_CONV linear_InterpolationTerminate(void);



extern LIB_linear_Interpolation_C_API 
void MW_CALL_CONV linear_InterpolationPrintStackTrace(void);

extern LIB_linear_Interpolation_C_API 
bool MW_CALL_CONV mlxLinear_Interpolation(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                          *prhs[]);

extern LIB_linear_Interpolation_C_API 
bool MW_CALL_CONV mlxLink(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_linear_Interpolation_C_API 
bool MW_CALL_CONV mlxSerialLink(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_linear_Interpolation
#define PUBLIC_linear_Interpolation_CPP_API __declspec(dllexport)
#else
#define PUBLIC_linear_Interpolation_CPP_API __declspec(dllimport)
#endif

#define LIB_linear_Interpolation_CPP_API PUBLIC_linear_Interpolation_CPP_API

#else

#if !defined(LIB_linear_Interpolation_CPP_API)
#if defined(LIB_linear_Interpolation_C_API)
#define LIB_linear_Interpolation_CPP_API LIB_linear_Interpolation_C_API
#else
#define LIB_linear_Interpolation_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV linear_Interpolation(int nargout, mwArray& dataS, const mwArray& x2, const mwArray& y2, const mwArray& z2, const mwArray& wx2, const mwArray& wy2, const mwArray& wz2);

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV Link(int nargout, mwArray& varargout, const mwArray& varargin);

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV Link(int nargout, mwArray& varargout);

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV Link(const mwArray& varargin);

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV Link();

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV SerialLink(int nargout, mwArray& varargout, const mwArray& varargin);

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV SerialLink(int nargout, mwArray& varargout);

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV SerialLink(const mwArray& varargin);

extern LIB_linear_Interpolation_CPP_API void MW_CALL_CONV SerialLink();

#endif
#endif
