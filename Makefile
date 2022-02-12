.SUFFIXES: .asm .d

TARGET = -t apple2enh
LIB = --lib apple2enh.lib
CSRC = main.c base.c
CFLAGS = --cpu 65c02 -Cl -T -Osir

all: a.out

a.out: $(CSRC:.c=.o)
	ld65 $(TARGET) -S 24576 -m a.map -vm $(CSRC:.c=.o) $(LIB)
	dcc6502 -c -d -s 58 -o 24576 a.out > a.lst
	java -jar ac.jar -d a.dsk T
	java -jar ac.jar -as a.dsk T BIN < a.out
	java -jar ac.jar -ls a.dsk

.c.o:
	cc65 $(TARGET) $(CFLAGS) --create-dep depend_tmp -o $(<:.c=.s) $<
	ca65 $(TARGET) $(<:.c=.s) -o $(<:.c=.o) -l $(<:.c=.lst)
	perl -ne 'print if s/s:/o:/;' depend_tmp > $(<:.c=.d)

clean:
	rm -f a.{map,out} *.{d,lst,o,s} depend_tmp

ifneq ($(MAKECMDGOALS),clean)
-include $(CSRC:.c=.d)
endif
