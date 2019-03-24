#ifndef UNRAR_LOCALE_H
#define UNRAR_LOCALE_H 1

/*
** unrar_locale.h
**
** (c) 2006 by Guido Mersmann
**
** Object source created by SimpleCat
*/

/*************************************************************************/

#include "locale_strings.h"
#include <wchar.h>

/*
** Prototypes
*/

BOOL Locale_Open( STRPTR catname, ULONG version, ULONG revision);
void Locale_Close(void);
STRPTR GetString(long ID);
const wchar_t *GetWString(long id);

/*************************************************************************/

#endif /* UNRAR_LOCALE_H */
