#include "rar.hpp"
#include "unicode.hpp"
#include <iconv.h>
#include <utf8proc.h>

int iconvConversionError = 0;
int iconvOpenError = 0;

iconv_t convBaseFromUtf8 = 0;
iconv_t convBaseToUtf8 = 0;

#define DEFAULT_CODEPAGE "ISO-8859-1"
#define CENTRAL_EUROPE_CODEPAGE "ISO-8859-2"
#define CYRILIC_CODEPAGE "Windows-1251"
#define GREEK_CODEPAGE "ISO-8859-7"


const char *DetectCodePage()
{
  char *lang = getenv("Language");
  if (!lang) return DEFAULT_CODEPAGE;
  
  if (strcmp(lang, "polski") == 0 ||
      strcmp(lang, "czech") == 0 ||
      strcmp(lang, "hrvatski") == 0 ||
      strcmp(lang, "magyar") == 0 ||
      strcmp(lang, "slovak") == 0 ||
      strcmp(lang, "slovensko") == 0 ||
      strcmp(lang, "srpski") == 0 ||
      strcmp(lang, "romana") == 0) {
    
    return CENTRAL_EUROPE_CODEPAGE;
    }
    
  if (strcmp(lang, "russian") == 0 ||
      strcmp(lang, "ukrainian") == 0 ||
      strcmp(lang, "bulgarian") == 0 ||
      strcmp(lang, "belarusian") == 0) {
    return CYRILIC_CODEPAGE;
  }
  
  if (strcmp(lang, "greek") == 0) {
    return GREEK_CODEPAGE;
  }
  
  return DEFAULT_CODEPAGE;
  
}


const char *GetCodePage()
{
  static char *rar_codepage = getenv("RAR_CODEPAGE");
#ifdef __amigaos4__
  static char *codepage = getenv("Charset");
#elif defined __AROS__
  static char *codepage = getenv("CHARSET");
#else
  static char *codepage = getenv("CODEPAGE");
#endif
  return (rar_codepage)?rar_codepage:(codepage)?codepage:DetectCodePage();
}


const wchar_t *GetCodePageW()
{
  static wchar_t *rar_codepage = 0;
  if (!rar_codepage)
  {
    rar_codepage = (wchar *)malloc(256);
    swprintf(rar_codepage, 255, L"%s", GetCodePage());
  }
  return rar_codepage;
}


bool InitConvBaseFromUtf8()
{
  convBaseFromUtf8=iconv_open(GetCodePage(), "UTF-8");
  if (convBaseFromUtf8 == (iconv_t)-1)
  {
	iconvOpenError = 1;
    convBaseFromUtf8=iconv_open(DEFAULT_CODEPAGE, "UTF-8");
    if (convBaseFromUtf8 == (iconv_t)-1)
    {
	  return false;
    }
  }
  return true;
}


bool InitConvBaseToUtf8()
{
  convBaseToUtf8=iconv_open("UTF-8", GetCodePage());
  if (convBaseToUtf8 == (iconv_t)-1)
  {
	iconvOpenError = 1;
    convBaseToUtf8=iconv_open("UTF-8", DEFAULT_CODEPAGE);
    if (convBaseToUtf8 == (iconv_t)-1)
    {
	  return false;
    }
  }
  return true;
}


void ReleaseConvBase()
{
  if (convBaseToUtf8)
  {
    iconv_close(convBaseToUtf8);
  }
  if (convBaseFromUtf8)
  {
    iconv_close(convBaseFromUtf8);
  }
}


bool WideToLocal(const wchar *Src,char *Dest,size_t DestSize)
{
  // buffer for UTF-8 version of Src
  unsigned char utf8Buf[NM];
  WideToUtf(Src,(char *)utf8Buf,ASIZE(utf8Buf));

  // normalizing UTF-8
  unsigned char *lineBufNorm = utf8proc_NFC(utf8Buf);

  // converting normalized UTF-8 to local encoding

  // init iconv
  if (!convBaseFromUtf8 && !InitConvBaseFromUtf8())
  {
    // can't do anything more - let's just return UTF-8
    WideToUtf(Src,Dest,DestSize);
    return true;
  }

  // perform conversion from UTF-8
#if defined(__AROS__)
  char *inPtr = (char *)lineBufNorm;
#else
  const char *inPtr = (const char *)lineBufNorm;
#endif
  char *outPtr = Dest;
  size_t inSize = strlen((char *)lineBufNorm);
  size_t outSize = DestSize;
  while (iconv(convBaseFromUtf8, &inPtr, &inSize, &outPtr, &outSize) == (size_t)-1)
  {
    if (!outSize || !inSize) break;
    iconvConversionError = 1;
    *outPtr = '?';
    outPtr++;
    outSize--;
    int chw = 1;
    unsigned char cp = (unsigned char)(*inPtr);
    if (cp < 0x80)
    {
      chw = 1;
    }
    else if (cp < 0xe0)
    {
      chw = 2;
    }
    else if (cp < 0xf0)
    {
      chw = 3;
    }
    else
    {
      chw = 4;
    }
    inPtr += chw;
    inSize -= chw;
    if (!outSize || !inSize) break;
  }
  *outPtr = 0;
  //iconv_close(convBase);
  free(lineBufNorm);

  return true;
}


bool LocalToWide(const char *Src,wchar *Dest,size_t DestSize)
{
  // init iconv
  if (!convBaseToUtf8 && !InitConvBaseToUtf8())
  {
    // can't do anything more - let's just return what we have
    return UtfToWide(Src, Dest, DestSize);
  }
  
  // perform conversion to UTF8
  char buf[4 * DestSize + 1];
#if defined(__AROS__)
  char *inPtr = (char *)Src;
#else
  const char *inPtr = Src;
#endif
  char *outPtr = (char *)buf;
  size_t inSize = strlen((char *)Src);
  size_t outSize = 4 * DestSize;
  while (iconv(convBaseToUtf8, &inPtr, &inSize, &outPtr, &outSize) == (size_t)-1)
  {
    if (!outSize || !inSize) break;
    iconvConversionError = 1;
    *outPtr = '?';
    outPtr++;
    outSize--;
    inPtr++;
    inSize--;
    if (!outSize || !inSize) break;
  }
  *outPtr = 0;
  //iconv_close(convBase);

  // convert to wide Unicode
  return UtfToWide(buf, Dest, DestSize);
}


void AmigaPathToUnix(const wchar_t *apath, wchar_t *upath)
{
  // this function translates amiga path into unix path
  // it does the following transformations:
  // 1) trailing '/' is replaced with '../'
  // 2) each '//' substring is replaced with '/../'
  
  int a = 0;  // amiga pointer
  int u = 0;  // unix pointer 
  while (apath[a])
  {
    if (a == 0 && apath[0] ==  '/')
    {
      upath[0] = '.';
	  upath[1] = '.';
	  upath[2] = '/';
	  u += 2;
    }
    else if (apath[a] == '/' && apath[a+1] == '/')
    {
      upath[u] = '/';
	  upath[u + 1] = '.';
	  upath[u + 2] = '.';
      u += 2;
    }
    else if (apath[a] == '/')
    {
      upath[u] = '/';
    }
    else
    {
      upath[u] = apath[a];
    }
    u++;
    a++;
  }
  upath[u] = 0;
}


void UnixPathToAmiga(const char *upath, char *apath)
{
  // this function translates unix path into amiga path
  // it does the following transformations:
  // 1) trailing '/' is replaced with ':'
  // 2) each '../' substring is replaced with '/'
  // 3) remove './' when there is just one '.'
  
  int a = 0;  // amiga pointer
  int u = 0;  // unix pointer 
  while (upath[u])
  {
    if (u == 0 && upath[0] ==  '/')
    {
      apath[0] = ':';
    }
    else if (upath[u] == '.' && upath[u+1] == '.' && upath[u+2] == '/')
    {
      apath[a] = '/';
      u += 2;
    }
    else if (upath[u] == '.' && upath[u+1] == '/')
    {
      u++;
    }
    else if (upath[u] == '/')
    {
      apath[a] = '/';
    }
    else
    {
      apath[a] = upath[u];
    }
    // skip all repeated '/'
    if (upath[u] == '/')
    {
      while (upath[u+1] == '/') u++;
    }
    u++;
    a++;
  }
  apath[a] = 0;
}

