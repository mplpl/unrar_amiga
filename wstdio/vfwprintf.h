#ifndef __VFWPRINTF_H__
#define __VFWPRINTF_H__

extern "C" int fwprintf(FILE *stream,const wchar_t *format,...);
extern "C" int wprintf(const wchar_t *format, ...);
extern "C" int swprintf(wchar_t *s, size_t, const wchar_t *format, ...);
extern "C" int vfwprintf(FILE *stream,const wchar_t *format,va_list arg);
extern "C" int vwprintf(const wchar_t *format,va_list arg);
extern "C" int vswprintf(wchar_t *s, size_t, const wchar_t *format,va_list arg);
extern "C" long wcstol (const wchar_t *nptr, wchar_t **endptr,int base);
extern "C" int putwchar(wchar_t wc);

#endif

