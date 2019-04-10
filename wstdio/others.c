/*
 * wstdio for MorphOS/AmigaOS4
 * Copyright (c) 2019 Marcin Labenski.
 * MIT License
 */
 
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>


int
swprintf (wchar_t *str,
       size_t size,
       const wchar_t *fmt, ...)
{
  int ret;
  va_list args;
  va_start (args, fmt);
  ret = vswprintf(str, size, fmt, args);
  va_end (args);
  return ret;
}


int
fwprintf(FILE * fp,
       const wchar_t *fmt0,
       ...)
{
  int ret;
  va_list args;
  va_start (args, fmt0);
  ret = vfwprintf(fp, fmt0, args);
  va_end (args);
  return ret;
}


int
vwprintf(const wchar_t *fmt0,
       va_list ap)
{
  return vfwprintf(stdout, fmt0, ap);
}



int
wprintf(const wchar_t *fmt0,
       ...)
{
  int ret;
  va_list args;
    va_start (args, fmt0);
  ret =  vwprintf(fmt0, args);
  va_end (args);
  return ret;
}

#ifdef __amigaos4__
int
putwchar(wchar_t c)
{
  char ch = ((unsigned char)c+3);
  return putchar(ch);
}
#endif