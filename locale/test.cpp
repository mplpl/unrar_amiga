#include "loclangamiga.hpp"
#include <stdio.h>

#ifdef __amigaos4__
#include "../vfwprintf/vfwprintf.h"
#endif

int main()
{
  Locale_Open("unrar.catalog", 1, 0);
  wprintf(L"%ls\n", MUCopyright);
  wprintf(L"%ls\n", MUCopyright);
  Locale_Close();
}
