#include "rar.hpp"
#include "unicode.hpp"
#include <iconv.h>
#include <utf8proc.h>

#define AMIGA_REL_DATE 14.3.2022

#ifdef __mini__
size_t __stack  = 600000;
#else
size_t __stack  = 800000;
#endif
static const char __attribute((used)) min_stack[] = "$STACK:800000";
#define Q(x) #x
#define QUOTE(x) Q(x)
#if RARVER_MINOR < 10
static const char __attribute((used)) vers[] = "\\0$VER: UnRAR "
		QUOTE(RARVER_MAJOR) ".0" QUOTE(RARVER_MINOR)
        " (" QUOTE(AMIGA_REL_DATE) ")";
#else
static const char __attribute((used)) vers[] = "\\0$VER: UnRAR "
		QUOTE(RARVER_MAJOR) "." QUOTE(RARVER_MINOR)
        " (" QUOTE(AMIGA_REL_DATE) ")";
#endif

#include <proto/exec.h>
int Check_Stack()
{
#ifdef __amigaos4__
	return 0;
#elif defined(__MORPHOS__) && !defined(__warpos__)
	return 0;
#elif defined(__warpos__) 
  printf("\nWARNING: stack must be 800K or more - before running unrar, call: stack 800000\n");
  return 0;
  /*struct TaskPPC *task = FindTaskPPC(NULL);
  printf("Stack check fired %u \n", task->tp_StackSize);
	if (task->tp_StackSize < __stack)
	{
		printf("Stack too small %d - %d bytes is needed\n",
			task->tp_StackSize,
			__stack);
		return 1;
	}
	return 0;*/
#else
  struct Task *task = FindTask(NULL);
  size_t sz = (size_t)task->tc_SPUpper - (size_t)task->tc_SPLower;
  if (sz < __stack)
	{
		printf("Stack too small %d - %d bytes is needed\n",
			sz,
			__stack);
		return 1;
	}
	return 0;
#endif
}

int iconvConversionError = 0;
int iconvOpenError = 0;

iconv_t convBaseFromUtf8 = (iconv_t)-1;
iconv_t convBaseToUtf8 = (iconv_t)-1;

#define DEFAULT_CODEPAGE "ISO-8859-1"
#define CENTRAL_EUROPE_CODEPAGE "ISO-8859-2"
#define ALT_CENTRAL_EUROPE_CODEPAGE "ISO-8859-16"
#define MALTESE_CODEPAGE "ISO-8859-3"
#define CYRILIC_CODEPAGE "Windows-1251"
#define ALT_CYRILIC_CODEPAGE "ISO-8859-5"
#define GREEK_CODEPAGE "ISO-8859-7"
#define HEBREW_CODEPAGE "ISO-8859-8"
#define TURKISH_CODEPAGE "ISO-8859-9"
#define BALTIC_CODEPAGE "ISO-8859-13"
#define THAI_CODEPAGE "TIS-620"


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
      strcmp(lang, "bosanski") == 0)
  {
    return CENTRAL_EUROPE_CODEPAGE;
  }

  if (strcmp(lang, "romanian") == 0)
  {
    return ALT_CENTRAL_EUROPE_CODEPAGE;
  }
    
  if (strcmp(lang, "russian") == 0)
  {
    return CYRILIC_CODEPAGE;
  }
  
  if (strcmp(lang, "ukrainian") == 0 ||
      strcmp(lang, "bulgarian") == 0 ||
      strcmp(lang, "belarusian") == 0 ||
      strcmp(lang, "macedonian") == 0
      ) {
    return ALT_CYRILIC_CODEPAGE;
  }

  if (strcmp(lang, "greek") == 0) {
    return GREEK_CODEPAGE;
  }

  if (strcmp(lang, "hebrew") == 0) {
    return HEBREW_CODEPAGE;
  }

  if (strcmp(lang, "estonian") == 0 ||
      strcmp(lang, "latvian") == 0 ||
      strcmp(lang, "lithuanian") == 0)
  {
    return BALTIC_CODEPAGE;
  }

  if (strcmp(lang, "maltese") == 0 ||
      strcmp(lang, "esperanto") == 0)
  {
    return MALTESE_CODEPAGE;
  }

  if (strcmp(lang, "türkçe") == 0)
  {
    return TURKISH_CODEPAGE;
  }

  if (strcmp(lang, "thai") == 0)
  {
    return THAI_CODEPAGE;
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

  if (rar_codepage) return rar_codepage;         // use code page set explicitely
  if (codepage) return codepage;                 // use code page set by OS
  static const char *detectedCodePage = DetectCodePage();
  return detectedCodePage;                       // use detected code page based on language
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
  if (convBaseToUtf8 != (iconv_t)-1)
  {
    iconv_close(convBaseToUtf8);
  }
  if (convBaseFromUtf8 != (iconv_t)-1)
  {
    iconv_close(convBaseFromUtf8);
  }
}

#if defined(__amigaos4__)

bool DetectConversionError(const char *utf8, const char *local)
{
  // iconv in AOS4 does not stop on wrong characters, but it
  // replaces them with '?' and moves on
  // because of that, I don't know if encoding is ok or not
  // this function tries to detect that, by checking number of
  // '?' in strings before and after conversion - it it does not
  // match, there was an error in conversion

  int marker1 = 0;
  int marker2 = 0;
  int i = -1;
  while (utf8[++i]) if (utf8[i] == '?') marker1++;
  i = -1;
  while (local[++i]) if (local[i] == '?') marker2++;
  return marker1 != marker2;
}

#endif

bool WideToLocal(const wchar *Src,char *Dest,size_t DestSize)
{
    
  // init iconv
  if (convBaseFromUtf8 == (iconv_t)-1 && !InitConvBaseFromUtf8())
  {
    // can't do anything more - let's just return UTF-8
    WideToUtf(Src,Dest,DestSize);
    return true;
  }
  
  // buffer for UTF-8 version of Src
  unsigned char utf8Buf[NM];
  WideToUtf(Src,(char *)utf8Buf,ASIZE(utf8Buf));

  // normalizing UTF-8
  unsigned char *lineBufNorm = utf8proc_NFC(utf8Buf);

  // converting normalized UTF-8 to local encoding

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

#if defined(__amigaos4__)
  if (DetectConversionError((const char *)lineBufNorm, Dest))
  {
    iconvConversionError = 1;
  }
#endif

  free(lineBufNorm);

  return true;
}


bool LocalToWide(const char *Src,wchar *Dest,size_t DestSize)
{
  // init iconv
  if (convBaseToUtf8 == (iconv_t)-1 && !InitConvBaseToUtf8())
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

#if defined(__amigaos4__)
  if (DetectConversionError(buf, Src))
  {
    iconvConversionError = 1;
  }
#endif

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

