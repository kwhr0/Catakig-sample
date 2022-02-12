#include "base.h"
#include <stdlib.h>

#define STAR_N		16
#define SHIFT		5
#define NEAR		(1 << SHIFT)
#define FAR			127
#define CX			140
#define CY			96
#define INVALID		-128
#define DURATION	6

static void clockpos(u8 pos) { CLOCK = pos; } // to avoid wrong optimization

static u8 printspeed() {
	static u16 cnt;
	static u8 c;
	clockpos(20); // lower digit of second
	if (c != CLOCK) {
		c = CLOCK;
		_log("%d loops/sec.\n", cnt);
		cnt = 1;
		return 1;
	}
	cnt++;
	return 0;
}

static u16 R(u16 n) {
	return (long)n * rand() >> 15;
}

static void text_demo() {
	u8 c, i;
	init(MODE_TXT80);
	for (i = 0; i < DURATION; i += printspeed()) {
		setTAttr(0);
		for (c = 32; c < 128; c++) _putc(c);
		setTAttr(1);
		for (c = 32; c < 96; c++) _putc(c);
		setTAttr(2);
		for (c = 32; c < 96; c++) _putc(c);
	}
}

static void point2_demo(point2_func *fp, u8 m) {
	u8 i;
	init(MODE_LORES);
	for (i = 0; i < DURATION / 2; i += printspeed()) {
		setGAttr(rand());
		(*fp)(R(40), R(48), R(40), R(48));
	}
	init(MODE_HIRES);
	for (i = 0; i < DURATION / 2; i += printspeed()) {
		setGAttr(rand() | m);
		(*fp)(R(280), R(192), R(280), R(192));
	}
}

static void lores_mix_demo() {
	u8 a = 0, c = 32, i, x = 20, y = 20;
	init(MODE_LORES);
	mix(1);
	for (i = 0; i < DURATION; i += printspeed()) {
		setGAttr(++a);
		for (c = 32; c < 128; c++) {
			point(x, y);
			switch (rand() & 3) {
				case 0: if (x > 0) x--; break;
				case 1: if (x < 39) x++; break;
				case 2: if (y > 0) y--; break;
				case 3: if (y < 39) y++; break;
			}
			_putc(c);
		}
	}
}

static void hires_dblbuf_demo() {
	typedef struct {
		s8 x, y, z;
	} Star;
	static Star star[STAR_N];
	static u8 *erase[2][STAR_N];
	Star *p;
	u8 i;
	u16 d, z;
	init(MODE_HIRES);
	d = (FAR - NEAR << 8) / STAR_N;
	z = NEAR << 8;
	for (p = star; p < &star[STAR_N]; p++) {
		p->x = INVALID;
		p->z = (z += d) >> 8;
	}
	for (i = 0; i < DURATION; i += printspeed()) {
		u8 **q, **ql;
		u8 sw = vramSwap();
		ql = &erase[sw][STAR_N];
		for (q = erase[sw]; q < ql; q++) if (*q) **q = 0;
		for (p = star, q = erase[sw]; p < &star[STAR_N]; p++, q++) {
			if (p->z > NEAR) p->z--;
			else {
				p->x = (rand() >> 7) - 128;
				p->y = (3 * (rand() & 0x1fff) >> 7) - 96;
				p->z = FAR;
			}
			if (p->x != INVALID) {
				u16 x = (p->x << SHIFT) / p->z + CX;
				u8 y = (p->y << SHIFT) / p->z + CY;
				setGAttr(p->z | 3);
				*q = point(x, y);
			}
			else *q = 0;
		}
	}
}

int main() {
	srand(0x1234);
	while (1) {
		point2_func l = line, b = box;
		text_demo();
		point2_demo(&l, 3);
		point2_demo(&b, 3);
		point2_demo(&boxfill, 0);
		lores_mix_demo();
		hires_dblbuf_demo();
	}
	return 0;
}
