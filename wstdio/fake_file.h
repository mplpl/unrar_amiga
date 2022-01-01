#ifndef __FAKE_FILE_H__
#define __FAKE_FILE_H__

#include <stdio.h>

typedef struct FAKE_FILE_ {
	char head[7];
	unsigned char *_base;
	unsigned long _size;
	unsigned char *_p;
	unsigned long _w;
	
	 
} FAKE_FILE;

#ifdef __cplusplus
extern "C" {
#endif

void init_fake_file(FAKE_FILE *ff, unsigned char *base, unsigned long size);
int is_fake_file(FILE *f); 

#ifdef __cplusplus
}
#endif

#endif
