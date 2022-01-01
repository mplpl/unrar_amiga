#include <stdio.h>

#define __USE_INLINE__
#include <proto/dos.h>

int truncate(const char *path, off_t length)
{
  int retval = 0;
  BPTR fd = Open(path, MODE_OLDFILE);
  if (fd != 0) 
  {
    retval = (SetFileSize(fd, length, OFFSET_BEGINNING) >= 0) ? 0 : -1;
    Close(fd);
  }
  return retval;
}