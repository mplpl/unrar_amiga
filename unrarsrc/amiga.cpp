

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
