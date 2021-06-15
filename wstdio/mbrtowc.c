#include <newlib.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "local.h"


typedef struct mbstate0_value {
   unsigned char __wchb[4];
} mbstate0_value_t;

typedef struct mbstate0 {
   int __count;
   struct {
       unsigned char __wchb[4];
   } __value;
} mbstate0_t;


int
__utf8_mbtowc (wchar_t *pwc,
        const char    *s,
        size_t         n,
		mbstate_t      *state0)
{
  wchar_t dummy;
  unsigned char *t = (unsigned char *)s;
  int ch;
  int i = 0;
  mbstate0_t *state = (mbstate0_t*)state0;

  if (pwc == NULL)
    pwc = &dummy;

  if (s == NULL)
    return 0;

  if (n == 0)
    return -2;

  if (state->__count == 0)
    ch = t[i++];
  else
    ch = state->__value.__wchb[0];

  if (ch == '\0')
    {
      *pwc = 0;
      state->__count = 0;
      return 0; /* s points to the null character */
    }

  if (ch <= 0x7f)
    {
      /* single-byte sequence */
      state->__count = 0;
      *pwc = ch;
      return 1;
    }
  if (ch >= 0xc0 && ch <= 0xdf)
    {
      /* two-byte sequence */
      state->__value.__wchb[0] = ch;
      if (state->__count == 0)
	state->__count = 1;
      else if (n < (size_t)-1)
	++n;
      if (n < 2)
	return -2;
      ch = t[i++];
      if (ch < 0x80 || ch > 0xbf)
	{
	  return -1;
	}
      if (state->__value.__wchb[0] < 0xc2)
	{
	  /* overlong UTF-8 sequence */
	  return -1;
	}
      state->__count = 0;
      *pwc = (wchar_t)((state->__value.__wchb[0] & 0x1f) << 6)
	|    (wchar_t)(ch & 0x3f);
      return i;
    }
  if (ch >= 0xe0 && ch <= 0xef)
    {
      /* three-byte sequence */
      wchar_t tmp;
      state->__value.__wchb[0] = ch;
      if (state->__count == 0)
	state->__count = 1;
      else if (n < (size_t)-1)
	++n;
      if (n < 2)
	return -2;
      ch = (state->__count == 1) ? t[i++] : state->__value.__wchb[1];
      if (state->__value.__wchb[0] == 0xe0 && ch < 0xa0)
	{
	  /* overlong UTF-8 sequence */
	  return -1;
	}
      if (ch < 0x80 || ch > 0xbf)
	{
	  return -1;
	}
      state->__value.__wchb[1] = ch;
      if (state->__count == 1)
	state->__count = 2;
      else if (n < (size_t)-1)
	++n;
      if (n < 3)
	return -2;
      ch = t[i++];
      if (ch < 0x80 || ch > 0xbf)
	{
	  return -1;
	}
      state->__count = 0;
      tmp = (wchar_t)((state->__value.__wchb[0] & 0x0f) << 12)
	|    (wchar_t)((state->__value.__wchb[1] & 0x3f) << 6)
	|     (wchar_t)(ch & 0x3f);
      *pwc = tmp;
      return i;
    }
  if (ch >= 0xf0 && ch <= 0xf4)
    {
      /* four-byte sequence */
      wint_t tmp;
      state->__value.__wchb[0] = ch;
      if (state->__count == 0)
	state->__count = 1;
      else if (n < (size_t)-1)
	++n;
      if (n < 2)
	return -2;
      ch = (state->__count == 1) ? t[i++] : state->__value.__wchb[1];
      if ((state->__value.__wchb[0] == 0xf0 && ch < 0x90)
	  || (state->__value.__wchb[0] == 0xf4 && ch >= 0x90))
	{
	  /* overlong UTF-8 sequence or result is > 0x10ffff */
	  return -1;
	}
      if (ch < 0x80 || ch > 0xbf)
	{
	  return -1;
	}
      state->__value.__wchb[1] = ch;
      if (state->__count == 1)
	state->__count = 2;
      else if (n < (size_t)-1)
	++n;
      if (n < 3)
	return -2;
      ch = (state->__count == 2) ? t[i++] : state->__value.__wchb[2];
      if (ch < 0x80 || ch > 0xbf)
	{
	  return -1;
	}
      state->__value.__wchb[2] = ch;
      if (state->__count == 2)
	state->__count = 3;
      else if (n < (size_t)-1)
	++n;
      if (state->__count == 3 && sizeof(wchar_t) == 2)
	{
	  /* On systems which have wchar_t being UTF-16 values, the value
	     doesn't fit into a single wchar_t in this case.  So what we
	     do here is to store the state with a special value of __count
	     and return the first half of a surrogate pair.  The first
	     three bytes of a UTF-8 sequence are enough to generate the
	     first half of a UTF-16 surrogate pair.  As return value we
	     choose to return the number of bytes actually read up to
	     here.
	     The second half of the surrogate pair is returned in case we
	     recognize the special __count value of four, and the next
	     byte is actually a valid value.  See below. */
	  tmp = (wint_t)((state->__value.__wchb[0] & 0x07) << 18)
	    |   (wint_t)((state->__value.__wchb[1] & 0x3f) << 12)
	    |   (wint_t)((state->__value.__wchb[2] & 0x3f) << 6);
	  state->__count = 4;
	  *pwc = 0xd800 | ((tmp - 0x10000) >> 10);
	  return i;
	}
      if (n < 4)
	return -2;
      ch = t[i++];
      if (ch < 0x80 || ch > 0xbf)
	{
	  return -1;
	}
      tmp = (wint_t)((state->__value.__wchb[0] & 0x07) << 18)
	|   (wint_t)((state->__value.__wchb[1] & 0x3f) << 12)
	|   (wint_t)((state->__value.__wchb[2] & 0x3f) << 6)
	|   (wint_t)(ch & 0x3f);
      if (state->__count == 4 && sizeof(wchar_t) == 2)
	/* Create the second half of the surrogate pair for systems with
	   wchar_t == UTF-16 . */
	*pwc = 0xdc00 | (tmp & 0x3ff);
      else
	*pwc = tmp;
      state->__count = 0;
      return i;
    }

  return -1;
}

size_t
mbrtowc (wchar_t *pwc,
	const char *s,
	size_t n,
	mbstate_t *ps)
{
  int retval = 0;
  if (s == NULL)
	retval = __utf8_mbtowc(NULL, "", 1, ps);
  else
	retval = __utf8_mbtowc(pwc, s, n, ps);

  if (retval == -1)
    {
	  ((mbstate0_t*)ps)->__count = 0;
      return (size_t)(-1);
    }
  else
    return (size_t)retval;
}

