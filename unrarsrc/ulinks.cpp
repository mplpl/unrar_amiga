
#ifdef _AMIGA
#include <dos/dos.h>
// including proto/dos.h make a mess due to overlapping type names
// therefore I'm just including these two declarations
extern "C" BOOL MakeLink(STRPTR, STRPTR, LONG);
extern "C" LONG SetFileDate( CONST_STRPTR, CONST struct DateStamp *);

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
    while (upath[u+1] == '/') u++;
    u++;
    a++;
  }
  apath[a] = 0;
}


#endif

static bool UnixSymlink(const char *Target,const wchar *LinkName,RarTime *ftm,RarTime *fta)
{
#ifdef _AMIGA
  CreatePath(LinkName,true);
  DelFile(LinkName);
  char LinkNameA[NM];
  WideToLocal(LinkName,LinkNameA,ASIZE(LinkNameA));
  char TargetAmiga[NM];
  UnixPathToAmiga(Target, TargetAmiga);
  printf("[ami path %s]", TargetAmiga);
  if (!(MakeLink(LinkNameA, (char *)TargetAmiga, 1)))
  {
    uiMsg(UIERROR_SLINKCREATE,UINULL,LinkName);
    ErrHandler.SetErrorCode(RARX_WARNING);
    return false;
  }
  if (ftm!=NULL && ftm->IsSet())
  {
    struct DateStamp ds;
    time_t amitime = ftm->GetUnix() - 252457200; // Amiga counts from Jan 1 78
    ds.ds_Days = amitime / (60 * 60 * 24);
    ds.ds_Minute = amitime % (60 * 60 * 24) / 60;
    ds.ds_Tick = amitime % 60 * TICKS_PER_SECOND;
    SetFileDate(LinkNameA, &ds);
  }
#else
  CreatePath(LinkName,true);
  DelFile(LinkName);
  char LinkNameA[NM];
  WideToChar(LinkName,LinkNameA,ASIZE(LinkNameA));
  if (symlink(Target,LinkNameA)==-1) // Error.
  {
    if (errno==EEXIST)
      uiMsg(UIERROR_ULINKEXIST,LinkName);
    else
    {
      uiMsg(UIERROR_SLINKCREATE,UINULL,LinkName);
      ErrHandler.SetErrorCode(RARX_WARNING);
    }
    return false;
  }
#ifdef USE_LUTIMES
#ifdef UNIX_TIME_NS
  timespec times[2];
  times[0].tv_sec=fta->GetUnix();
  times[0].tv_nsec=fta->IsSet() ? long(fta->GetUnixNS()%1000000000) : UTIME_NOW;
  times[1].tv_sec=ftm->GetUnix();
  times[1].tv_nsec=ftm->IsSet() ? long(ftm->GetUnixNS()%1000000000) : UTIME_NOW;
  utimensat(AT_FDCWD,LinkNameA,times,AT_SYMLINK_NOFOLLOW);
#else
  struct timeval tv[2];
  tv[0].tv_sec=fta->GetUnix();
  tv[0].tv_usec=long(fta->GetUnixNS()%1000000000/1000);
  tv[1].tv_sec=ftm->GetUnix();
  tv[1].tv_usec=long(ftm->GetUnixNS()%1000000000/1000);
  lutimes(LinkNameA,tv);
#endif
#endif

#endif
  
  return true;
}


static bool IsFullPath(const char *PathA) // Unix ASCII version.
{
  return *PathA==CPATHDIVIDER;
}


bool ExtractUnixLink30(CommandData *Cmd,ComprDataIO &DataIO,Archive &Arc,const wchar *LinkName)
{
  char Target[NM];
  if (IsLink(Arc.FileHead.FileAttr))
  {
    size_t DataSize=(size_t)Arc.FileHead.PackSize;
    if (DataSize>ASIZE(Target)-1)
      return false;
    if ((size_t)DataIO.UnpRead((byte *)Target,DataSize)!=DataSize)
      return false;
    Target[DataSize]=0;

    DataIO.UnpHash.Init(Arc.FileHead.FileHash.Type,1);
    DataIO.UnpHash.Update(Target,strlen(Target));
    DataIO.UnpHash.Result(&Arc.FileHead.FileHash);

    // Return true in case of bad checksum, so link will be processed further
    // and extraction routine will report the checksum error.
    if (!DataIO.UnpHash.Cmp(&Arc.FileHead.FileHash,Arc.FileHead.UseHashKey ? Arc.FileHead.HashKey:NULL))
      return true;

    wchar TargetW[NM];
    CharToWide(Target,TargetW,ASIZE(TargetW));
    // Check for *TargetW==0 to catch CharToWide failure.
    // Use Arc.FileHead.FileName instead of LinkName, since LinkName
    // can include the destination path as a prefix, which can
    // confuse IsRelativeSymlinkSafe algorithm.
    if (!Cmd->AbsoluteLinks && (*TargetW==0 || IsFullPath(TargetW) ||
        !IsRelativeSymlinkSafe(Cmd,Arc.FileHead.FileName,LinkName,TargetW)))
      return false;
    return UnixSymlink(Target,LinkName,&Arc.FileHead.mtime,&Arc.FileHead.atime);
  }
  return false;
}


bool ExtractUnixLink50(CommandData *Cmd,const wchar *Name,FileHeader *hd)
{
  char Target[NM];
#ifdef _AMIGA
  WideToLocal(hd->RedirName,Target,ASIZE(Target));
#else
  WideToChar(hd->RedirName,Target,ASIZE(Target));
#endif
  if (hd->RedirType==FSREDIR_WINSYMLINK || hd->RedirType==FSREDIR_JUNCTION)
  {
    // Cannot create Windows absolute path symlinks in Unix. Only relative path
    // Windows symlinks can be created here. RAR 5.0 used \??\ prefix
    // for Windows absolute symlinks, since RAR 5.1 /??/ is used.
    // We escape ? as \? to avoid "trigraph" warning
    if (strncmp(Target,"\\??\\",4)==0 || strncmp(Target,"/\?\?/",4)==0)
      return false;
    DosSlashToUnix(Target,Target,ASIZE(Target));
  }
  // Use hd->FileName instead of LinkName, since LinkName can include
  // the destination path as a prefix, which can confuse
  // IsRelativeSymlinkSafe algorithm.
  if (!Cmd->AbsoluteLinks && (IsFullPath(Target) ||
      !IsRelativeSymlinkSafe(Cmd,hd->FileName,Name,hd->RedirName)))
    return false;
  return UnixSymlink(Target,Name,&hd->mtime,&hd->atime);
}
