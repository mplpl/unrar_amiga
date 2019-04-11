/*
 * wstdio for MorphOS/AmigaOS4
 * Copyright (c) 2019 Marcin Labenski.
 * MIT License
 */
 
#ifndef __VFWPRINTF_H__
#define __VFWPRINTF_H__

#ifdef __cplusplus
extern "C" {
#endif

int fwprintf(FILE *stream,const wchar_t *format,...);
int wprintf(const wchar_t *format, ...);
int swprintf(wchar_t *s, size_t, const wchar_t *format, ...);
int vfwprintf(FILE *stream,const wchar_t *format,va_list arg);
int vwprintf(const wchar_t *format,va_list arg);
int vswprintf(wchar_t *s, size_t, const wchar_t *format,va_list arg);
long wcstol (const wchar_t *nptr, wchar_t **endptr,int base);
int putwchar(wchar_t wc);

#ifdef __cplusplus
}
#endif

#endif

