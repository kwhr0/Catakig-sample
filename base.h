#include "types.h"

enum {
	MODE_TXT40, MODE_TXT80, MODE_LORES, MODE_HIRES
};

typedef u8 *(*point_func)(u16 x, u8 y);
typedef void (*point2_func)(u16 x0, u8 y0, u16 x1, u8 y1);

extern point_func point;
extern point2_func boxfill;

void _putc(u8 ch);
void _log(const char *format, ...);
void init(u8 mode);
void mix(u8 f);
u8 vramSwap(void);
void setGAttr(u8 a);
void setTAttr(u8 a);
void setPrinter(u8 f);
void line(u16 x0, u8 y0, u16 x1, u8 y1);
void box(u16 x0, u8 y0, u16 x1, u8 y1);
