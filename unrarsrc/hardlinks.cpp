#ifdef _AMIGA
#define __USE_INLINE__
#include <proto/dos.h>

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


LONG SetFileModificationTime(const char *path, RarTime *ftm)
{
  if (ftm && ftm->IsSet())
  {
    struct DateStamp ds;
    time_t amitime = ftm->GetUnix() - 252457200; // Amiga counts from Jan 1 78
    ds.ds_Days = amitime / (60 * 60 * 24);
    ds.ds_Minute = amitime % (60 * 60 * 24) / 60;
    ds.ds_Tick = amitime % 60 * TICKS_PER_SECOND;
    return SetFileDate(path, &ds);
  }
  return 0;
}

#endif

bool ExtractHardlink(wchar *NameNew,wchar *NameExisting,size_t NameExistingSize,Archive &Arc)
{
  SlashToNative(NameExisting,NameExisting,NameExistingSize); // Not needed for RAR 5.1+ archives.

  if (!FileExist(NameExisting))
  {
    uiMsg(UIERROR_HLINKCREATE,NameNew);
    uiMsg(UIERROR_NOLINKTARGET);
    ErrHandler.SetErrorCode(RARX_CREATE);
    return false;
  }
  CreatePath(NameNew,true);

#ifdef _WIN_ALL
  bool Success=CreateHardLink(NameNew,NameExisting,NULL)!=0;
  if (!Success)
  {
    uiMsg(UIERROR_HLINKCREATE,NameNew);
    ErrHandler.SysErrMsg();
    ErrHandler.SetErrorCode(RARX_CREATE);
  }
  return Success;
#elif defined(_AMIGA)
  char NameExistingA[NM],NameNewA[NM],NameExistingAmiga[NM];
  WideToLocal(NameExisting,NameExistingA,ASIZE(NameExistingA));
  WideToLocal(NameNew,NameNewA,ASIZE(NameNewA));
  UnixPathToAmiga(NameExistingA, NameExistingAmiga);
  BPTR lock = Lock(NameExistingAmiga, SHARED_LOCK);
  bool Success = false;
  if (lock)
  {
    Success = (MakeLink(NameNewA, (LONG)lock, false));
    if (!Success)
    {
      uiMsg(UIERROR_HLINKCREATE,NameNew);
      ErrHandler.SetErrorCode(RARX_CREATE);
    }
    UnLock(lock);
  }
  SetFileModificationTime(NameNewA, &Arc.FileHead.mtime);
  return Success;
#elif defined(_UNIX)
  char NameExistingA[NM],NameNewA[NM];
  WideToChar(NameExisting,NameExistingA,ASIZE(NameExistingA));
  WideToChar(NameNew,NameNewA,ASIZE(NameNewA));
  bool Success=link(NameExistingA,NameNewA)==0;
  if (!Success)
  {
    uiMsg(UIERROR_HLINKCREATE,NameNew);
    ErrHandler.SysErrMsg();
    ErrHandler.SetErrorCode(RARX_CREATE);
  }
  return Success;
#else
  return false;
#endif
}

