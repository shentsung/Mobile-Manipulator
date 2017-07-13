//
// MATLAB Compiler: 5.1 (R2014a)
// Date: Mon Jul 10 11:16:49 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:lineInterpolation" "-T"
// "link:lib" "-d"
// "C:\Users\yanglei\Desktop\lineInterpolation\lineInterpolation\for_testing"
// "-v" "C:\Users\yanglei\Desktop\lineInterpolation\lineInterpolation.m"
// "C:\Users\yanglei\Desktop\lineInterpolation\Link.m"
// "C:\Users\yanglei\Desktop\lineInterpolation\SerialLink.m"
// "class{Class1:C:\Users\yanglei\Desktop\lineInterpolation\lineInterpolation.m,
// C:\Users\yanglei\Desktop\lineInterpolation\Link.m,C:\Users\yanglei\Desktop\li
// neInterpolation\SerialLink.m}" 
//

#ifndef __lineInterpolation_h
#define __lineInterpolation_h

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

#ifdef EXPORTING_lineInterpolation
#define PUBLIC_lineInterpolation_C_API __global
#else
#define PUBLIC_lineInterpolation_C_API /* No import statement needed. */
#endif

#define LIB_lineInterpolation_C_API PUBLIC_lineInterpolation_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_lineInterpolation
#define PUBLIC_lineInterpolation_C_API __declspec(dllexport)
#else
#define PUBLIC_lineInterpolation_C_API __declspec(dllimport)
#endif

#define LIB_lineInterpolation_C_API PUBLIC_lineInterpolation_C_API


#else

#define LIB_lineInterpolation_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_lineInterpolation_C_API 
#define LIB_lineInterpolation_C_API /* No special import/export declaration */
#endif

extern LIB_lineInterpolation_C_API 
bool MW_CALL_CONV lineInterpolationInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_lineInterpolation_C_API 
bool MW_CALL_CONV lineInterpolationInitialize(void);

extern LIB_lineInterpolation_C_API 
void MW_CALL_CONV lineInterpolationTerminate(void);



extern LIB_lineInterpolation_C_API 
void MW_CALL_CONV lineInterpolationPrintStackTrace(void);

extern LIB_lineInterpolation_C_API 
bool MW_CALL_CONV mlxLineInterpolation(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                       *prhs[]);

extern LIB_lineInterpolation_C_API 
bool MW_CALL_CONV mlxLink(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_lineInterpolation_C_API 
bool MW_CALL_CONV mlxSerialLink(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_lineInterpolation
#define PUBLIC_lineInterpolation_CPP_API __declspec(dllexport)
#else
#define PUBLIC_lineInterpolation_CPP_API __declspec(dllimport)
#endif

#define LIB_lineInterpolation_CPP_API PUBLIC_lineInterpolation_CPP_API

#else

#if !defined(LIB_lineInterpolation_CPP_API)
#if defined(LIB_lineInterpolation_C_API)
#define LIB_lineInterpolation_CPP_API LIB_lineInterpolation_C_API
#else
#define LIB_lineInterpolation_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV lineInterpolation(int nargout, mwArray& VL, const mwArray& x0, const mwArray& y0, const mwArray& theta1, const mwArray& theta2, const mwArray& theta3, const mwArray& theta4, const mwArray& theta5, const mwArray& theta6, const mwArray& theta7, const mwArray& detax, const mwArray& detay, const mwArray& detaz, const mwArray& deta_alpha, const mwArray& deta_beta, const mwArray& deta_gamma);

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV Link(int nargout, mwArray& varargout, const mwArray& varargin);

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV Link(int nargout, mwArray& varargout);

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV Link(const mwArray& varargin);

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV Link();

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV SerialLink(int nargout, mwArray& varargout, const mwArray& varargin);

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV SerialLink(int nargout, mwArray& varargout);

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV SerialLink(const mwArray& varargin);

extern LIB_lineInterpolation_CPP_API void MW_CALL_CONV SerialLink();

#endif
#endif
