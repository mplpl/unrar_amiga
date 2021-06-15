#ifdef _AMIGA
#define __USE_INLINE__
#include <proto/dos.h>

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

bool ExtractHardlink(CommandData *Cmd,wchar *NameNew,wchar *NameExisting,size_t NameExistingSize)
{
  SlashToNative(NameExisting,NameExisting,NameExistingSize); // Not needed for RAR 5.1+ archives.

  if (!FileExist(NameExisting))
  {
    uiMsg(UIERROR_HLINKCREATE,NameNew);
    uiMsg(UIERROR_NOLINKTARGET);
    ErrHandler.SetErrorCode(RARX_CREATE);
    return false;
  }
  CreatePath(NameNew,true,Cmd->DisableNames);

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
  WideToChar(NameExisting,NameExistingA,ASIZE(NameExistingA));
  WideToChar(NameNew,NameNewA,ASIZE(NameNewA));
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

