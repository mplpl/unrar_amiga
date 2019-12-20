#include "loclangamiga.hpp"
#include <stdio.h>

#if defined(__amigaos4__) || defined(__AROS__)
#include "../wstdio/wstdio.h"
#endif

int main()
{
  Locale_Open("unrar.catalog");
  wprintf(L"%ls\n", MAmigaPortBy);
  wprintf(L"%ls\n", MAmigaPortBy);
  Locale_Close();
}
