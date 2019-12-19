
#include "fake_file.h"

void init_fake_file(FAKE_FILE *ff, unsigned char *base, unsigned long size)
{

	ff->head[0] = '#';
	ff->head[1] = 'F';
	ff->head[2] = 'A';
	ff->head[3] = 'K';
	ff->head[4] = 'E';
	ff->head[5] = '#';
	ff->head[6] = 0;
	
	ff->_base = ff->_p = base;
	ff->_size = ff->_w = size;
}

int is_fake_file(FILE *f)
{
	unsigned char *c = (unsigned char *)f;
	return c[0] == '#' && c[1] == 'F' && c[2] == 'A' && c[3] == 'K' 
		&& c[4] == 'E' && c[5] == '#';

}