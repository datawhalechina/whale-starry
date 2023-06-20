/** @file stl_def.h
 *  This is an define header file
 */

#define __YAMI_BEGIN namespace Yami {
#define __YAMI_END }

// Macro for noexcept, to support in mixed 03/0x mode.
#ifndef _T_STD_NOEXCEPT
#if __cplusplus >= 201103L
#define _T_STD_NOEXCEPT noexcept
#define _T_STD_USE_NOEXCEPT noexcept
#define _T_STD_THROW(_EXC)
#else
#define _T_STD_NOEXCEPT
#define _T_STD_USE_NOEXCEPT throw()
#define _T_STD_THROW(_EXC) throw(_EXC)
#endif
#endif