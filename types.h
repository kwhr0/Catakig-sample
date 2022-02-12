#ifndef _TYPES_H_
#define _TYPES_H_

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef volatile u8 vu8;

#define _80STOREOFF	(*(vu8 *)0xc000)
#define _80STOREON	(*(vu8 *)0xc001)
#define CLR80VID	(*(vu8 *)0xc00c)
#define SET80VID	(*(vu8 *)0xc00d)
#define TXTCLR		(*(vu8 *)0xc050)
#define TXTSET		(*(vu8 *)0xc051)
#define MIXCLR		(*(vu8 *)0xc052)
#define MIXSET		(*(vu8 *)0xc053)
#define TXTPAGE1	(*(vu8 *)0xc054)
#define TXTPAGE2	(*(vu8 *)0xc055)
#define LORES		(*(vu8 *)0xc056)
#define HIRES		(*(vu8 *)0xc057)
#define CLOCK		(*(vu8 *)0xc0bf)
#define PUTCHAR		(*(vu8 *)0xc0ff)	// customized

#endif
