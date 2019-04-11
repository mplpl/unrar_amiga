/*
 * wstdio for MorphOS/AmigaOS4
 * Copyright (c) 2019 Marcin Labenski.
 * MIT License
 */

#ifndef __UNRAR_LOCALE_H__
#define __UNRAR_LOCALE_H__


#include "locale_strings.h"
#include <wchar.h>

BOOL Locale_Open(const char *catname);
void Locale_Close();
STRPTR GetString(long ID);
const wchar_t *GetWString(long id);

#endif
