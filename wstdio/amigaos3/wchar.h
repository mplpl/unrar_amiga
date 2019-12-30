#ifndef _WCHAR_H
#define _WCHAR_H


#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WEOF (-1)
typedef unsigned int wint_t;
typedef long mbstate_t;

int fwprintf(FILE *stream,const wchar_t *format,...);
int wprintf(const wchar_t *format, ...);
int swprintf(wchar_t *s, size_t, const wchar_t *format, ...);
int vfwprintf(FILE *stream,const wchar_t *format,va_list arg);
int vwprintf(const wchar_t *format,va_list arg);
int vswprintf(wchar_t *s, size_t, const wchar_t *format,va_list arg);
long wcstol (const wchar_t *nptr, wchar_t **endptr,int base);
int putwchar(wchar_t wc);
int wcscmp(const wchar_t *s1, const wchar_t *s2);
size_t wcslen(const wchar_t *s);
wchar_t *wcscpy(wchar_t *s1, const wchar_t *s2);
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wcschr(const wchar_t *s, wchar_t c);
wchar_t *wcspbrk(const wchar_t *s, const wchar_t *set);
wchar_t *wcsrchr(const wchar_t *s, wchar_t c);
wchar_t *wcsncpy(wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wmemmove(wchar_t *d, const wchar_t *s, size_t n);
wchar_t *wmemset(wchar_t *s, wchar_t c, size_t n);
size_t wcrtomb(char *s, wchar_t wc, mbstate_t *ps);
size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps);
wint_t towupper(wint_t x);
wint_t towlower(wint_t c);

#ifdef __cplusplus
}
#endif 

#endif
