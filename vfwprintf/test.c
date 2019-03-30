#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>

int main()
{
    setlocale(LC_ALL,"");
	
	//wprintf(L"Start\n");
	/*wprintf(L"Test %ls\n", L"wprintf");
    fwprintf(stdout, L"Test %ls\n", L"fwprintf");
    wprintf(L"Test %ls\n", L"vfwprintf");
	go(stdout, L"test %d %d %s %ls\n", 13, 2, "abc", L"qwe");*/
	/*
	wprintf(L"Test %ls\n", L"wcstol");
	wchar_t len[10];
	wchar_t *rest;
	int x = wcstol(L"1234ABC", &rest, 10);
	printf("%d\n", x);
	wprintf(L"%ls\n", rest);
	
	wprintf(L"Test %ls\n", L"swprintf");
	wchar_t Version[80];
	swprintf(Version,ASIZE(Version),L"%d.%02d",5,5);
	wprintf(Version);
	
	wchar_t buf2[2];
	swprintf(buf2,ASIZE(buf2),L"%d.%02d",5,5);
	wprintf(buf2);
	*/
	/*wprintf(L"Test vswprintf (simple): ");
	wchar_t buf3[1024];
	swprintf(buf3, 1024, L"to jest test %s Z\n", "123");
	wprintf(buf3);*/
	//puts("aaa");
  //fputws(L"aaa", stdout);
	wprintf(L"Test vswprintf: ");
	wchar_t buf4[1024];
	swprintf(buf4, 1024, L"to jest test %ls\n", L"a\x017c\x00f3\x0142\x0107z");
	wprintf(buf4);
	
	/*
	FILE *f = fopen("x.tmp", "w+");
	fwide(f, 1);
	if (f)
	{
		fputc('x', f);
		int ret = fputwc(L'\x017c', f);
		if (ret == WEOF) printf("BBBBB");
		fclose(f);
	} else printf("can't open");*/
	
	//printf("stdout %d\n", fwide(stdout, 0));
}
