#include <newlib.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "local.h"


size_t
wcrtomb (char *__restrict s,
	wchar_t wc,
	mbstate_t *__restrict ps)
{
#if defined(PREFER_SIZE_OVER_SPEED) || defined(__OPTIMIZE_SIZE__)
  return _wcrtomb_r (s, wc, ps);
#else
  int retval = 0;
  char buf[10];



  if (s == NULL)
    retval = wctomb (buf, L'\0');
  else
    retval = wctomb (s, wc);

  if (retval == -1)
    {
      //ps->__count = 0;
      return (size_t)(-1);
    }
  else
    return (size_t)retval;
#endif /* not PREFER_SIZE_OVER_SPEED */
}

