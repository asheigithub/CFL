#ifdef ANDROID

#ifndef _CLEAR_WINDOWS
#define _CLEAR_WINDOWS


#ifdef _WIN32
#undef _WIN32
#undef WIN32
#undef __cplusplus
#define __cplusplus 201103L

#define __inline__  inline
#define __signed__  signed
#define __builtin_va_list  char*
#define __builtin_memchr memchr
#define __builtin_memcmp memcmp
#define __builtin_strlen strlen
#define __builtin_memmove memmove
#define __builtin_memcpy memcpy
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define __builtin_va_start(ap,v)  ( ap = (va_list)&(v) + _INTSIZEOF(v) ) 
#define __builtin_va_end(ap)      ( ap = (va_list)0 )
#define __WINT_TYPE__ int
#define __signed signed
#define __sighandler_t int

typedef unsigned short char16_t;
typedef unsigned int char32_t;
typedef long double __float128;


#endif

#endif // !_CLEAR_WINDOWS


#endif