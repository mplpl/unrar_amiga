/*
 * Copyright (c) 1990, 2007 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	%W% (UofMD/Berkeley) %G%
 */

/*
 * Information local to this implementation of stdio,
 * in particular, macros and private variables.
 */

#include "_ansi.h"
//#include <reent.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#ifdef __SCLE
# include <io.h>
#endif


/* Called by the main entry point fns to ensure stdio has been initialized.  */

#define CHECK_INIT(fp) 
#define CHECK_STD_INIT(ptr)

/* Return true and set errno and stream error flag iff the given FILE
   cannot be written now.  */

#define	cantwrite(fp)                                     \
  ((((fp)->_flags & __SWR) == 0 || (fp)->_bf._base == NULL) && \
   __swsetup(fp))

/* Test whether the given stdio file has an active ungetc buffer;
   release such a buffer, without restoring ordinary unread data.  */

#define	HASUB(fp) ((fp)->_ub._base != NULL)
#define	FREEUB(fp) {                    \
	if ((fp)->_ub._base != (fp)->_ubuf) \
		free((char *)(fp)->_ub._base); \
	(fp)->_ub._base = NULL; \
}

/* Test for an fgetline() buffer.  */

#define	HASLB(fp) ((fp)->_lb._base != NULL)
#define	FREELB(fp) { free((char *)(fp)->_lb._base); \
      (fp)->_lb._base = NULL; }

#ifdef _WIDE_ORIENT
/*
 * Set the orientation for a stream. If o > 0, the stream has wide-
 * orientation. If o < 0, the stream has byte-orientation.
 */
#define ORIENT(fp,ori)					\
  do								\
    {								\
      if (!((fp)->_flags & __SORD))	\
	{							\
	  (fp)->_flags |= __SORD;				\
	  if (ori > 0)						\
	    (fp)->_flags2 |= __SWID;				\
	  else							\
	    (fp)->_flags2 &= ~__SWID;				\
	}							\
    }								\
  while (0)
#else
#define ORIENT(fp,ori)
#endif
