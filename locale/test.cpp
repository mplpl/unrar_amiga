#include "loclangamiga.hpp"
#include <stdio.h>

#ifdef __amigaos4__
#include "../wstdio/wstdio.h"
#endif

int main()
{
  Locale_Open("unrar.catalog", 1, 0);
  wprintf(L"%ls\n", MAmigaPortBy);
  wprintf(L"%ls\n", MAmigaPortBy);
  Locale_Close();
}
