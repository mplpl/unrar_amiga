
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

#define CATCOMP_BLOCK 0
#define CATCOMP_NUMBERS 1
#define CATCOMP_ARRAY 1
#include "unrar_locale.h"

#include <string.h>
#include <wchar.h>
#include <stdlib.h>

#define EXPECTED_CATALOG_VERSION 6
#define EXPECTED_CATALOG_REVISION 2

/*************************************************************************/

#if defined(__amigaos4__)
struct Library     *LocaleBase;
struct LocaleIFace *ILocale;
#elif defined(__amigaos3__) || defined(__warpos__)
struct LocaleBase  *LocaleBase;
#elif !defined(__AROS__)
struct Library     *LocaleBase;
#endif

static struct Locale      *locale_locale;
static struct Catalog     *locale_catalog;

static wchar_t *CACHE[CATCOMP_LASTID + 1];


/*************************************************************************/

BOOL Locale_Open(const char *catname)
{
  memset(CACHE, 0, sizeof(wchar_t) * (CATCOMP_LASTID + 1));
#if defined(__AROS__) || defined(__amigaos3__) || defined(__warpos__) 
  if( (LocaleBase = (struct LocaleBase *)OpenLibrary("locale.library", 0)) )
#else
  if( (LocaleBase = OpenLibrary("locale.library", 0)) )
#endif
  {
#ifdef __amigaos4__
    if((ILocale = (struct LocaleIFace *)GetInterface(LocaleBase, "main", 1, NULL)))
    {
#endif
      if((locale_locale = OpenLocale(NULL)))
      {
        if((locale_catalog = OpenCatalog(locale_locale, (char *)catname, TAG_DONE)))
        {
          if(locale_catalog->cat_Version  == EXPECTED_CATALOG_VERSION  &&
              locale_catalog->cat_Revision == EXPECTED_CATALOG_REVISION) {
	          
            return(TRUE);
	        }

          CloseLocale(locale_locale);
          locale_locale = NULL;
        }
        CloseLocale(locale_locale);
        locale_locale = NULL;
      }
#ifdef __amigaos4__
      DropInterface((struct Interface *)ILocale);
    }
#endif
    CloseLibrary((struct Library *)LocaleBase);
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
    CloseLibrary((struct Library *)LocaleBase);
    LocaleBase = NULL;
  }
}


/*************************************************************************/

STRPTR GetString(long id)
{
LONG   *l;
UWORD  *w;
STRPTR  builtin;
int row = 0;
LONG tid;

  while (id != CatCompArray[row].cca_ID)
  {
     tid = CatCompArray[row].cca_ID;
     if (tid == CATCOMP_LASTID)
     {
     	return (STRPTR)"x";
     }
     row++;
  }
  builtin = CatCompArray[row].cca_Str;

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



