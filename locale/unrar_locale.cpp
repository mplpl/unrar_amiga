
/*
** unrar_locale.c
**
** (c) 2011 by Guido Mersmann
**
** Object source created by SimpleCat.
**
** DO NOT EDIT THIS FILE UNLESS YOU KNOW WHAT YOU ARE DOING
**
** Usage is quite simple.
**
** 1. Add this file to object creation list in makefile.
**
** 2. Call Locale_Open() and Locale_Close() on startup and shut down of your code.
**    There is no need to check any result.
**
** 3. Make sure the marked include line within 'unrar_locale.h' header file uses
**    the name of your string header file. Thats name in .CS file which is set with
**    TARGET C header file command or when using the GUI in target list. By default
**    its "locale_strings.h". Make sure you use the following switches for the C
**    target to create a valid header file for this code:
**    NoCode NoArray NoBlockStatic
**
** 4. Include the 'unrar_locale.h' header everywhere you need a locale string.
**
** 5. Call GetString( id ) where ever you need a locale string. "id" is the label name
**    you address the strings. To follow the style guide you should name the files
**    using "MSG_" prefix.
**
** 6. Thats all!
**
*/

#define __USE_INLINE__
#include <exec/types.h>
#include <libraries/locale.h>

#include <proto/locale.h>
#include <proto/exec.h>

#define CATCOMP_BLOCK 1     /* enable CATCOMP_BLOCK */
#define CATCOMP_NUMBERS 1
#include "unrar_locale.h" /* prototypes and catcomp block */

#include <string.h>
#include <wchar.h>
#include <stdlib.h>

/*************************************************************************/

#ifdef __amigaos4__
struct Library     *LocaleBase;
struct LocaleIFace *ILocale;
#else
struct Library *LocaleBase;
#endif

static struct Locale      *locale_locale;
static struct Catalog     *locale_catalog;

static wchar_t *CACHE[CATCOMP_LASTID + 1];

/*************************************************************************/

/* /// Locale_Open */

/*************************************************************************/

BOOL Locale_Open( STRPTR catname, ULONG version, ULONG revision) 
{
  memset(CACHE, 0, sizeof(wchar_t) * (CATCOMP_LASTID + 1));
  
  if( (LocaleBase = OpenLibrary( "locale.library",0 )) ) 
  {
#ifdef __amigaos4__
    if( (ILocale = (struct LocaleIFace *) GetInterface( LocaleBase, "main", 1, NULL)) ) 
    {
#endif
      if( (locale_locale = OpenLocale(NULL)) ) 
      {
        if( (locale_catalog = OpenCatalog(locale_locale, catname, TAG_DONE)) )
        {
          if(    locale_catalog->cat_Version  == version  &&
            locale_catalog->cat_Revision == revision ) 
          {
              return(TRUE);
          }
          CloseCatalog(locale_catalog);
          locale_catalog = NULL;
        }
        CloseLocale(locale_locale);
        locale_locale = NULL;
      }
#ifdef __amigaos4__
      DropInterface((struct Interface *)ILocale);
    }
#endif
    CloseLibrary( LocaleBase );
    LocaleBase = NULL;
  }
  return(FALSE);
}
/* \\\ Locale_Open */

/* /// Locale_Close */

/*************************************************************************/

void Locale_Close(void)
{
  for (int i = 0; i < CATCOMP_LASTID; i++)
  {
    delete[] CACHE[i];
    CACHE[i] = 0;
  }
  if( LocaleBase) 
  {
    if( locale_catalog)
    {
      CloseCatalog(locale_catalog);
      locale_catalog = NULL;
    }
    if( locale_locale)
    {
      CloseLocale(locale_locale);
      locale_locale = NULL;
    }
#ifdef __amigaos4__
    DropInterface((struct Interface *)ILocale);
#endif
    CloseLibrary( LocaleBase );
    LocaleBase = NULL;
  }
}
/* \\\ */

/* /// GetString */

/*************************************************************************/

STRPTR GetString(long id)
{
LONG   *l;
UWORD  *w;
STRPTR  builtin;

  l = (LONG *)CatCompBlock;

  while (*l != id ) {
    w = (UWORD *)((ULONG)l + 4);
    l = (LONG *)((ULONG)l + (ULONG)*w + 6);
  }
  builtin = (STRPTR)((ULONG)l + 6);

  if ( locale_catalog && LocaleBase ) {
    return((STRPTR)GetCatalogStr( locale_catalog, id, builtin));
  }
  return(builtin);
}
/* \\\ GetString */

/* /// GetWString */

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

/* \\\ GetWString */



