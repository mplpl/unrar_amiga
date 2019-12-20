/*
FUNCTION
<<wctomb>>---minimal wide char to multibyte converter

INDEX
	wctomb

SYNOPSIS
	#include <stdlib.h>
	int wctomb(char *<[s]>, wchar_t <[wchar]>);

DESCRIPTION
When _MB_CAPABLE is not defined, this is a minimal ANSI-conforming 
implementation of <<wctomb>>.  The
only ``wide characters'' recognized are single bytes,
and they are ``converted'' to themselves.  

When _MB_CAPABLE is defined, this routine calls <<_wctomb_r>> to perform
the conversion, passing a state variable to allow state dependent
decoding.  The result is based on the locale setting which may
be restricted to a defined set of locales.

Each call to <<wctomb>> modifies <<*<[s]>>> unless <[s]> is a null
pointer or _MB_CAPABLE is defined and <[wchar]> is invalid.

RETURNS
This implementation of <<wctomb>> returns <<0>> if
<[s]> is <<NULL>>; it returns <<-1>> if _MB_CAPABLE is enabled
and the wchar is not a valid multi-byte character, it returns <<1>>
if _MB_CAPABLE is not defined or the wchar is in reality a single
byte character, otherwise it returns the number of bytes in the
multi-byte character.

PORTABILITY
<<wctomb>> is required in the ANSI C standard.  However, the precise
effects vary with the locale.

<<wctomb>> requires no supporting OS subroutines.
*/



#include <newlib.h>
#include <stdlib.h>
#include <errno.h>
#include "local.h"
#include <wchar.h>

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
__utf8_wctomb (char          *s,
        wchar_t        _wchar,
        mbstate0_t     *state)
{
  wint_t wchar = _wchar;
  int ret = 0;

  if (s == NULL)
    return 0; /* UTF-8 encoding is not state-dependent */

  if (sizeof (wchar_t) == 2 && state->__count == -4
      && (wchar < 0xdc00 || wchar > 0xdfff))
    {
      /* There's a leftover lone high surrogate.  Write out the CESU-8 value
	 of the surrogate and proceed to convert the given character.  Note
	 to return extra 3 bytes. */
      wchar_t tmp;
      tmp = (state->__value.__wchb[0] << 16 | state->__value.__wchb[1] << 8)
	    - (0x10000 >> 10 | 0xd80d);
      *s++ = 0xe0 | ((tmp & 0xf000) >> 12);
      *s++ = 0x80 | ((tmp &  0xfc0) >> 6);
      *s++ = 0x80 |  (tmp &   0x3f);
      state->__count = 0;
      ret = 3;
    }
  if (wchar <= 0x7f)
    {
      *s = wchar;
      return ret + 1;
    }
  if (wchar >= 0x80 && wchar <= 0x7ff)
    {
      *s++ = 0xc0 | ((wchar & 0x7c0) >> 6);
      *s   = 0x80 |  (wchar &  0x3f);
      return ret + 2;
    }
  if (wchar >= 0x800 && wchar <= 0xffff)
    {
      /* No UTF-16 surrogate handling in UCS-4 */
      if (sizeof (wchar_t) == 2 && wchar >= 0xd800 && wchar <= 0xdfff)
	{
	  wint_t tmp;
	  if (wchar <= 0xdbff)
	    {
	      /* First half of a surrogate pair.  Store the state and
	         return ret + 0. */
	      tmp = ((wchar & 0x3ff) << 10) + 0x10000;
	      state->__value.__wchb[0] = (tmp >> 16) & 0xff;
	      state->__value.__wchb[1] = (tmp >> 8) & 0xff;
	      state->__count = -4;
	      *s = (0xf0 | ((tmp & 0x1c0000) >> 18));
	      return ret;
	    }
	  if (state->__count == -4)
	    {
	      /* Second half of a surrogate pair.  Reconstruct the full
		 Unicode value and return the trailing three bytes of the
		 UTF-8 character. */
	      tmp = (state->__value.__wchb[0] << 16)
		    | (state->__value.__wchb[1] << 8)
		    | (wchar & 0x3ff);
	      state->__count = 0;
	      *s++ = 0xf0 | ((tmp & 0x1c0000) >> 18);
	      *s++ = 0x80 | ((tmp &  0x3f000) >> 12);
	      *s++ = 0x80 | ((tmp &    0xfc0) >> 6);
	      *s   = 0x80 |  (tmp &     0x3f);
	      return 4;
	    }
	  /* Otherwise translate into CESU-8 value. */
	}
      *s++ = 0xe0 | ((wchar & 0xf000) >> 12);
      *s++ = 0x80 | ((wchar &  0xfc0) >> 6);
      *s   = 0x80 |  (wchar &   0x3f);
      return ret + 3;
    }
  if (wchar >= 0x10000 && wchar <= 0x10ffff)
    {
      *s++ = 0xf0 | ((wchar & 0x1c0000) >> 18);
      *s++ = 0x80 | ((wchar &  0x3f000) >> 12);
      *s++ = 0x80 | ((wchar &    0xfc0) >> 6);
      *s   = 0x80 |  (wchar &     0x3f);
      return 4;
    }

  return -1;
}

int
wctomb (char *s,
        wchar_t wchar)
{
  mbstate0_t state;
  memset(&state, 0, sizeof(state));
  return __utf8_wctomb (s, wchar, &state);
}

int
wctombx (char *s,
        wchar_t wchar)
{
#ifdef _MB_CAPABLE
	struct _reent *reent = _REENT;

        _REENT_CHECK_MISC(reent);

        return __WCTOMB (reent, s, wchar, &(_REENT_WCTOMB_STATE(reent)));
#else /* not _MB_CAPABLE */
        if (s == NULL)
                return 0;

	/* Verify that wchar is a valid single-byte character.  */
	if ((size_t)wchar >= 0x100) {
		errno = EILSEQ;
		return -1;
	}
        *s = (char) wchar;
        return 1;
#endif /* not _MB_CAPABLE */

}

