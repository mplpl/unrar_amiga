
/*
 * Copyright (c) 2019 Marcin Labenski.
 * MIT License
 *
 * Original file was generated by SimpleCat (c) 2011 by Guido Mersmann
 *
 */

#define __USE_INLINE__
#include <exec/types.h>
#include <libraries/locale.h>

#include <proto/locale.h>
#include <proto/exec.h>

#define CATCOMP_BLOCK 1
#define CATCOMP_NUMBERS 1
#include "unrar_locale.h"

#include <string.h>
#include <wchar.h>
#include <stdlib.h>

/*************************************************************************/

#ifdef __amigaos4__
struct Library     *LocaleBase;
struct LocaleIFace *ILocale;
#else
struct Library     *LocaleBase;
#endif

static struct Locale      *locale_locale;
static struct Catalog     *locale_catalog;

static wchar_t *CACHE[CATCOMP_LASTID + 1];


/*************************************************************************/

BOOL Locale_Open(const char *catname)
{
  memset(CACHE, 0, sizeof(wchar_t) * (CATCOMP_LASTID + 1));
  
  if( (LocaleBase = OpenLibrary("locale.library", 0)) )
  {
#ifdef __amigaos4__
    if((ILocale = (struct LocaleIFace *)GetInterface(LocaleBase, "main", 1, NULL)))
    {
#endif
      if((locale_locale = OpenLocale(NULL)))
      {
        if((locale_catalog = OpenCatalog(locale_locale, catname, TAG_DONE)))
        {
          return(TRUE);
        }
        CloseLocale(locale_locale);
        locale_locale = NULL;
      }
#ifdef __amigaos4__
      DropInterface((struct Interface *)ILocale);
    }
#endif
    CloseLibrary(LocaleBase);
    LocaleBase = NULL;
  }
  return(FALSE);
}


/*************************************************************************/

void Locale_Close()
{
  for (int i = 0; i < CATCOMP_LASTID; i++)
  {
    delete[] CACHE[i];
    CACHE[i] = 0;
  }
  if(LocaleBase)
  {
    if(locale_catalog)
    {
      CloseCatalog(locale_catalog);
      locale_catalog = NULL;
    }
    if(locale_locale)
    {
      CloseLocale(locale_locale);
      locale_locale = NULL;
    }
#ifdef __amigaos4__
    DropInterface((struct Interface *)ILocale);
#endif
    CloseLibrary(LocaleBase);
    LocaleBase = NULL;
  }
}


/*************************************************************************/

STRPTR GetString(long id)
{
LONG   *l;
UWORD  *w;
STRPTR  builtin;

  l = (LONG *)CatCompBlock;

  while (*l != id ) 
  {
    w = (UWORD *)((ULONG)l + 4);
    l = (LONG *)((ULONG)l + (ULONG)*w + 6);
  }
  builtin = (STRPTR)((ULONG)l + 6);

  if (locale_catalog && LocaleBase)
  {
    return((STRPTR)GetCatalogStr(locale_catalog, id, builtin));
  }
  return(builtin);
}


/*************************************************************************/

extern bool LocalToWide(const char *Src,wchar_t *Dest,size_t DestSize);

const wchar_t *GetWString(long id)
{	
  if (id > CATCOMP_LASTID)
  {
    return 0;
  }

  if (CACHE[id])
  {
    return CACHE[id];
  }
  // new string
  const char *str = GetString(id);
  if (!str)
  {
    return 0;
  }

  // I have a string encoded using local charset of given locale
  // I need to convert it to wchar_t
  int len = strlen(str);
  CACHE[id] = new wchar_t[len + 1];
  LocalToWide(str, CACHE[id], len + 1);
  return CACHE[id];
} 



