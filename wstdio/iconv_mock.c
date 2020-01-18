#include "amigaos3/iconv.h"

iconv_t iconv_open (const char* tocode, const char* fromcode)
{
	return (iconv_t)-1;
}

size_t iconv (iconv_t cd, const char* * inbuf, size_t *inbytesleft, char* * outbuf, size_t *outbytesleft)
{
	return -1;
}

int iconv_close (iconv_t cd)
{
	return 0;
}
