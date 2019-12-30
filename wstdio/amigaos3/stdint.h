#ifndef	_STDINT_H
#define	_STDINT_H

typedef	signed char		int8_t;
typedef	signed short		int16_t;
typedef	signed int		int32_t;
typedef	signed long long	int64_t;
typedef	unsigned char		uint8_t;
typedef	unsigned short		uint16_t;
typedef	unsigned int		uint32_t;
typedef	unsigned long long	uint64_t;
typedef	signed long		intptr_t;
typedef	unsigned long		uintptr_t;
typedef	signed long long	intmax_t;
typedef	unsigned long long	uintmax_t;

#define	INT8_MIN		SCHAR_MIN
#define	INT8_MAX		SCHAR_MAX
#define	UINT8_MAX		UCHAR_MAX
#define	INT16_MIN		SHRT_MIN
#define	INT16_MAX		SHRT_MAX
#define	UINT16_MAX		USHRT_MAX
#define	INT32_MIN		INT_MIN
#define	INT32_MAX		INT_MAX
#define	UINT32_MAX		UINT_MAX
#define	INT64_MIN		LLONG_MIN
#define	INT64_MAX		LLONG_MAX
#define	UINT64_MAX		ULLONG_MAX
#define	INTPTR_MIN		LONG_MIN
#define	INTPTR_MAX		LONG_MAX
#define	UINTPTR_MAX		ULONG_MAX
#define	INTMAX_MIN		LLONG_MIN
#define	INTMAX_MAX		LLONG_MAX
#define	UINTMAX_MAX		ULLONG_MAX
#define	INTMAX_MIN		LLONG_MIN
#define	INTMAX_MAX		LLONG_MAX
#define	UINTMAX_MAX		ULLONG_MAX

#define	SIZE_MAX		UINT_MAX

#endif
