#ifndef _x11_framegrab_h
#define _x11_framegrab_h

#include <stdint.h>
#include <stdio.h>

#define MAX_OUTPUT_FORMATS 10

extern uint8_t * buffer;
extern int width, height;

extern const char * output_formats[MAX_OUTPUT_FORMATS];
extern int (*output_format_writer[MAX_OUTPUT_FORMATS])( );
extern int output_format_count;
extern int quality;
FILE * outputf;

#define REGISTER_OUTPUT_DRIVER(x) \
	void init##x() __attribute__((constructor)); \
	void init##x() \
	{ \
		output_formats[output_format_count] = #x; \
		output_format_writer[output_format_count++] = x##driver; \
	}

#endif


/*

Copyright (c) 2016 <>< Charles Lohr

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
