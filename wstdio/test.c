#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

int main()
{
  setlocale(LC_ALL,"");
  wchar_t buf4[1024];
  //swprintf(buf4, 1024, L"to jest test %ls\n", L"a\x017c\x00f3\x0142\x0107z");
  //wprintf(buf4);
  /*int i = 0;
  do {
  	putwchar(buf4[i]);
  } while (buf4[++i]) ; */
  
  //fputwc(L'±', stdout);
  //putwchar(L'±');
  wprintf(L"%c\n", L'x');
  fflush(stdout);
}

