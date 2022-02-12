#include "base.h"
#include <stdio.h>
#include <string.h>

point_func point;
point2_func boxfill;

static u16 vramofs, vramofs_p;
static u8 width80, mixed, cursX, cursY, gattr, tattr, vsw, printer;

static const u16 row[] = {
	0x400, 0x480, 0x500, 0x580, 0x600, 0x680, 0x700, 0x780,
	0x428, 0x4a8, 0x528, 0x5a8, 0x628, 0x6a8, 0x728, 0x7a8,
	0x450, 0x4d0, 0x550, 0x5d0, 0x650, 0x6d0, 0x750, 0x7d0,
};

static const u16 row8[] = {
	0x2000, 0x2080, 0x2100, 0x2180, 0x2200, 0x2280, 0x2300, 0x2380,
	0x2028, 0x20a8, 0x2128, 0x21a8, 0x2228, 0x22a8, 0x2328, 0x23a8,
	0x2050, 0x20d0, 0x2150, 0x21d0, 0x2250, 0x22d0, 0x2350, 0x23d0,
};

static const u8 ofs[] = {
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9,
	10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11,
	12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13,
	14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15,
	16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17,
	18, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 19, 19, 19,
	20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21,
	22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23,
	24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25,
	26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27,
	28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29,
	30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31,
	32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33,
	34, 34, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 35,
	36, 36, 36, 36, 36, 36, 36, 37, 37, 37, 37, 37, 37, 37,
	38, 38, 38, 38, 38, 38, 38, 39, 39, 39, 39, 39, 39, 39,
};

static const u8 mask[] = {
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
	129, 130, 132, 136, 144, 160, 192, 129, 130, 132, 136, 144, 160, 192,
};

static const u8 ctbl[][2] = {
	{ 0x00, 0x00 }, { 0x55, 0x2a }, { 0x2a, 0x55 }, { 0x7f, 0x7f },
	{ 0x80, 0x80 }, { 0xd5, 0xaa }, { 0xaa, 0xd5 }, { 0xff, 0xff },
};

int write(int /*fd*/, const void *buf, unsigned count) {
	char *p = (char *)buf;
	if (printer)
		while (count--) PUTCHAR = *p++;
	else
		while (count--) _putc(*p++);
	return 1; // SUCCESS
}

static void crlf() {
	u8 y;
	cursX = 0;
	if (++cursY < 24) return;
	y = mixed ? 20 : 0;
	if (width80) {
		for (; y < 23; y++) {
			TXTPAGE2 = 0;
			memcpy((u8 *)row[y], (u8 *)row[y + 1], 40);
			TXTPAGE1 = 0;
			memcpy((u8 *)row[y], (u8 *)row[y + 1], 40);
		}
		TXTPAGE2 = 0;
		memset((u8 *)0x7d0, 0xa0, 40);
		TXTPAGE1 = 0;
	}
	else
		for (; y < 23; y++)
			memcpy((u8 *)row[y], (u8 *)row[y + 1], 40);
	memset((u8 *)0x7d0, 0xa0, 40);
}

void _putc(u8 ch) {
	if (ch == 10) {
		crlf();
		return;
	}
	if (ch < 0x20 || ch > 0x7f) ch = 0x20;
	if (!(tattr & 0x80)) {
		if (ch > 0x5f) ch = 0x20;
		if (!(ch & 0x20)) ch ^= 0x40;
	}
	if (cursY > 23) cursY = 23;
	if (width80) {
		if (cursX & 1) TXTPAGE1 = 0;
		else TXTPAGE2 = 0;
		*((u8 *)row[cursY] + (cursX >> 1)) = ch | tattr;
		if (++cursX >= 80) crlf();
	}
	else {
		*((u8 *)row[cursY] + cursX) = ch | tattr;
		if (++cursX >= 40) crlf();
	}
}

void _log(const char *format, ...) {
	va_list ap;
	u8 last = printer;
	printer = 1;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printer = last;
}

static u8 *point_lo(u16 x, u8 y) {
	u8 *p = 0;
	if (x < 40 && y < 48) {
		p = (u8 *)row[y >> 1] + x + vramofs;
		*p = y & 1 ? *p & 0xf | gattr & 0xf0 : *p & 0xf0 | gattr & 0xf;
	}
	return p;
}

static u8 *point_hi(u16 x, u8 y) {
	u8 *p = 0;
	u8 r = y >> 3;
	if (x < sizeof(ofs) && r < sizeof(row8)) {
		u8 o = ofs[x], m = mask[x], d = ctbl[gattr & 7][o & 1];
		p = (u8 *)row8[r] + ((y & 7) << 10) + o + vramofs;
		*p = *p & ~m | d & m;
	}
	return p;
}

void line(u16 x0, u8 y0, u16 x1, u8 y1) {
	u16 x = x0, y = y0, dx, dy;
	s8 sx, sy;
	s16 d;
	if (x0 < x1) {
		dx = x1 - x0;
		sx = 1;
	}
	else {
		dx = x0 - x1;
		sx = -1;
	}
	if (y0 < y1) {
		dy = y1 - y0;
		sy = 1;
	}
	else {
		dy = y0 - y1;
		sy = -1;
	}
	if (dx > dy) {
		d = dx >> 1;
		while (1) {
			point(x, y);
			if ((d += dy) >= dx) {
				d -= dx;
				y += sy;
			}
			if (x == x1) return;
			x += sx;
		}
	}
	else {
		d = dy >> 1;
		while (1) {
			point(x, y);
			if ((d += dx) >= dy) {
				d -= dy;
				x += sx;
			}
			if (y == y1) return;
			y += sy;
		}
	}
}

static void boxfill_lo(u16 x0, u8 y0, u16 x1, u8 y1) {
	u16 x;
	u8 y, f;
	u8 *p;
	if (x0 > x1) {
		u16 t = x0;
		x0 = x1;
		x1 = t;
	}
	if (y0 > y1) {
		u8 t = y0;
		y0 = y1;
		y1 = t;
	}
	if (x0 > 39) x0 = 39;
	if (x1 > 39) x1 = 39;
	if (y0 > 47) y0 = 47;
	if (y1 > 47) y1 = 47;
	y = y0;
	if (y & 1) {
		p = (u8 *)row[y >> 1] + x0 + vramofs;
		for (x = x0; x <= x1; x++) {
			*p = *p & 0xf | gattr & 0xf0;
			p++;
		}
		y++;
	}
	f = !(y1 & 1);
	if (f) y1--;
	if (y1 != 255)
		for (; y <= y1; y++) {
			p = (u8 *)row[y >> 1] + x0 + vramofs;
			for (x = x0; x <= x1; x++) *p++ = gattr;
		}
	if (f) {
		p = (u8 *)row[y >> 1] + x0 + vramofs;
		for (x = x0; x <= x1; x++) {
			*p = *p & 0xf0 | gattr & 0xf;
			p++;
		}
	}
}

static void boxfill_hi(u16 x0, u8 y0, u16 x1, u8 y1) {
	u8 m0, m1, o0, o1, y;
	if (x0 > x1) {
		u16 t = x0;
		x0 = x1;
		x1 = t;
	}
	if (y0 > y1) {
		u8 t = y0;
		y0 = y1;
		y1 = t;
	}
	if (x0 > 279) x0 = 279;
	if (x1 > 279) x1 = 279;
	if (y0 > 191) y0 = 191;
	if (y1 > 191) y1 = 191;
	m0 = mask[x0];
	m1 = mask[x1];
	o0 = ofs[x0];
	o1 = ofs[x1];
	for (y = y0; y <= y1; y++) {
		u8 *p = (u8 *)row8[y >> 3] + ((y & 7) << 10) + o0 + vramofs;
		u8 d, m, o = o0;
		if (o < o1) {
			m = -m0 | 0x80;
			d = ctbl[gattr & 7][o & 1];
			*p = *p & ~m | d & m;
			p++;
			while (++o < o1)
				*p++ = ctbl[gattr & 7][o & 1];
			m = (m1 << 1) - 1 | 0x80;
		}
		else m = -m0 & (m1 << 1) - 1 | 0x80;
		d = ctbl[gattr & 7][o & 1];
		*p = *p & ~m | d & m;
	}
}

void box(u16 x0, u8 y0, u16 x1, u8 y1) {
	boxfill(x0, y0, x1, y0);
	boxfill(x0, y1, x1, y1);
	boxfill(x0, y0, x0, y1);
	boxfill(x1, y0, x1, y1);
}

void init(u8 mode) {
	cursX = cursY = mixed = vsw = printer = 0;
	tattr = 0x80; 
	switch (mode) {
	case MODE_TXT40:
		width80 = 0;
		memset((u8 *)0x400, 0xa0, 0x800);
		_80STOREOFF = 0;
		CLR80VID = 0;
		TXTPAGE1 = 0;
		TXTSET = 0;
		break;
	case MODE_TXT80:
		width80 = 1;
		_80STOREON = 0;
		TXTPAGE2 = 0;
		memset((u8 *)0x400, 0xa0, 0x400);
		TXTPAGE1 = 0;
		memset((u8 *)0x400, 0xa0, 0x400);
		SET80VID = 0;
		TXTSET = 0;
		break;
	case MODE_LORES:
		width80 = 0;
		gattr = 0xff;
		vramofs = 0;
		vramofs_p = 0x400;
		point = point_lo;
		boxfill = boxfill_lo;
		memset((u8 *)0x400, 0, 0x800);
		_80STOREOFF = 0;
		CLR80VID = 0;
		TXTPAGE1 = 0;
		TXTCLR = 0;
		LORES = 0;
		break;
	case MODE_HIRES:
		width80 = 0;
		gattr = 7;
		vramofs = 0;
		vramofs_p = 0x2000;
		point = point_hi;
		boxfill = boxfill_hi;
		memset((u8 *)0x2000, 0, 0x4000);
		_80STOREOFF = 0;
		CLR80VID = 0;
		TXTPAGE1 = 0;
		TXTCLR = 0;
		HIRES = 0;
		break;
	}
	MIXCLR = 0;
}

void mix(u8 f) {
	u8 c, i;
	mixed = f;
	if (f) {
		if (cursY < 20) cursY = 20;
		MIXSET = 0;
		c = 0xa0;
	}
	else {
		MIXCLR = 0;
		c = 0;
	}
	for (i = 20; i < 24; i++)
		memset((u8 *)row[i], c, 40);
}

u8 vramSwap() {
	vsw = !vsw;
	if (vsw) TXTPAGE2 = 0;
	else TXTPAGE1 = 0;
	vramofs = vsw ? 0 : vramofs_p;
	return vsw;
}

void setGAttr(u8 a) {
	gattr = a << 4 | a & 0xf;
}

void setTAttr(u8 a) {
	switch (a) {
	case 1:
		tattr = 0x00; // reverse
		break;
	case 2:
		tattr = 0x40; // blink
		break;
	default:
		tattr = 0x80; // normal
		break;
	}
}

void setPrinter(u8 f) {
	printer = f;
}
