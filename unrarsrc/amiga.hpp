#ifndef __UNRAR_AMIGA__HPP__
#define __UNRAR_AMIGA__HPP__

#include <utf8proc.h>

int Check_Stack();

extern int iconvConversionError;
extern int iconvOpenError;

const wchar_t *GetCodePageW();

void ReleaseConvBase();

bool WideToLocal(const wchar *Src,char *Dest,size_t DestSize);
bool LocalToWide(const char *Src,wchar *Dest,size_t DestSize);

void AmigaPathToUnix(const wchar_t *apath, wchar_t *upath);
void UnixPathToAmiga(const char *upath, char *apath);

#endif
