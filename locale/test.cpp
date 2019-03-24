#include "loclangamiga.hpp"
#include <stdio.h>

int main()
{
	Locale_Open("unrar.catalog", 1, 0);
	wprintf(L"%ls\n", MCopyright);
	wprintf(L"%ls\n", MCopyright);
	Locale_Close();
}