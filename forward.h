//
// MATLAB Compiler: 5.1 (R2014a)
// Date: Tue Jul 18 17:49:51 2017
// Arguments: "-B" "macro_default" "-W" "cpplib:forward" "-T" "link:lib" "-d"
// "C:\Users\yanglei\Desktop\forward\forward\for_testing" "-v"
// "C:\Users\yanglei\Desktop\forward\forward.m"
// "C:\Users\yanglei\Desktop\forward\Link.m"
// "C:\Users\yanglei\Desktop\forward\SerialLink.m"
// "class{Class1:C:\Users\yanglei\Desktop\forward\forward.m,C:\Users\yanglei\Des
// ktop\forward\Link.m,C:\Users\yanglei\Desktop\forward\SerialLink.m}" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\accel.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\animate.m
// " "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\cinertia.
// m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\coriolis.
// m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createBlo
// ckCGJacobianDH.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createBlo
// ckDirKinematicDH.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createBlo
// ckJacobianDH.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createCGJ
// acobianDH.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createDir
// KinematicDH.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createGen
// Accelerations.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createGen
// Coordinates.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createGen
// Forces.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createGen
// Velocities.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\createJac
// obianDH.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\fdyn.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\private\f
// indjobj.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\fkine.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\friction.
// m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\gencoords
// .m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\genforces
// .m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\gravload.
// m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\ikine.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\ikine3.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\ikine6s.m
// " "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\inertia.m
// " "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\issym.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\itorque.m
// " "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\jacob0.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\jacob_dot
// .m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\jacobn.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\maniplty.
// m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\nofrictio
// n.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\perturb.m
// " "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\plot.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\rne.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\rne_dh.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\rne_mdh.m
// " "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\SerialLin
// k.m" "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\slink.m"
// "-a"
// "C:\Study_Software\Matlab\R2014a\toolbox\rvctools\robot\@SerialLink\teach.m" 
//

#ifndef __forward_h
#define __forward_h 1

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

#ifdef EXPORTING_forward
#define PUBLIC_forward_C_API __global
#else
#define PUBLIC_forward_C_API /* No import statement needed. */
#endif

#define LIB_forward_C_API PUBLIC_forward_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_forward
#define PUBLIC_forward_C_API __declspec(dllexport)
#else
#define PUBLIC_forward_C_API __declspec(dllimport)
#endif

#define LIB_forward_C_API PUBLIC_forward_C_API


#else

#define LIB_forward_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_forward_C_API 
#define LIB_forward_C_API /* No special import/export declaration */
#endif

extern LIB_forward_C_API 
bool MW_CALL_CONV forwardInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_forward_C_API 
bool MW_CALL_CONV forwardInitialize(void);

extern LIB_forward_C_API 
void MW_CALL_CONV forwardTerminate(void);



extern LIB_forward_C_API 
void MW_CALL_CONV forwardPrintStackTrace(void);

extern LIB_forward_C_API 
bool MW_CALL_CONV mlxForward(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_forward_C_API 
bool MW_CALL_CONV mlxLink(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_forward_C_API 
bool MW_CALL_CONV mlxSerialLink(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_forward
#define PUBLIC_forward_CPP_API __declspec(dllexport)
#else
#define PUBLIC_forward_CPP_API __declspec(dllimport)
#endif

#define LIB_forward_CPP_API PUBLIC_forward_CPP_API

#else

#if !defined(LIB_forward_CPP_API)
#if defined(LIB_forward_C_API)
#define LIB_forward_CPP_API LIB_forward_C_API
#else
#define LIB_forward_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_forward_CPP_API void MW_CALL_CONV forward(int nargout, mwArray& fk, const mwArray& x, const mwArray& y, const mwArray& theta0, const mwArray& theta1, const mwArray& theta2, const mwArray& theta3, const mwArray& theta4, const mwArray& theta5, const mwArray& theta6);

extern LIB_forward_CPP_API void MW_CALL_CONV Link(int nargout, mwArray& varargout, const mwArray& varargin);

extern LIB_forward_CPP_API void MW_CALL_CONV Link(int nargout, mwArray& varargout);

extern LIB_forward_CPP_API void MW_CALL_CONV Link(const mwArray& varargin);

extern LIB_forward_CPP_API void MW_CALL_CONV Link();

extern LIB_forward_CPP_API void MW_CALL_CONV SerialLink(int nargout, mwArray& varargout, const mwArray& varargin);

extern LIB_forward_CPP_API void MW_CALL_CONV SerialLink(int nargout, mwArray& varargout);

extern LIB_forward_CPP_API void MW_CALL_CONV SerialLink(const mwArray& varargin);

extern LIB_forward_CPP_API void MW_CALL_CONV SerialLink();

#endif
#endif
