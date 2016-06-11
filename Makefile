all : x11framegrab

DRIVERS:=writer_ppm.c writer_stb.c writer_jpg.c
CFLAGS:=-Os -m32
LDFLAGS:=-lX11 -s -flto -ffunction-sections -Wl,--gc-sections

x11framegrab : x11framegrab.c $(DRIVERS)
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean :
	rm -rf x11framegrab
