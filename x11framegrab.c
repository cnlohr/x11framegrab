#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "x11framegrab.h"
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <unistd.h>

Display *display = 0;
Screen * screen = 0;
Window rootWindow = 0;
XWindowAttributes xwa;

uint8_t * buffer;
int width, height;

const char * output_formats[MAX_OUTPUT_FORMATS];
int (*output_format_writer[MAX_OUTPUT_FORMATS])( );
int output_format_count;
int quality = -1;


static int handler(Display * d, XErrorEvent *e	)
{
	fprintf( stderr, "FAIL: %p\n", d ) ;
	exit( -5 );
}


int CapScreen()
{
	int lx, ly;
	static int did_set_handler;

	if( !did_set_handler )
	{
		XSetErrorHandler( handler );
	}

	if( !display )
	{
		display = XOpenDisplay( 0 );
	   	screen = XDefaultScreenOfDisplay(display);
		rootWindow = DefaultRootWindow(display);
		XGetWindowAttributes(display, rootWindow, &xwa);
	}

	width = xwa.width;
	height = xwa.height;

	XImage * m = XGetImage( display, rootWindow, 0, 0, width, height, AllPlanes, XYPixmap);

	if( m->bitmap_pad != 32 )
	{
		fprintf( stderr, "Bad bitmap pad\n" );
		return -1;
	}

	uint8_t * bp = buffer = malloc( width * height * 3 );
	for( ly = 0; ly < height; ly++ )
	{
		for( lx = 0; lx < width; lx++ )
		{
			uint32_t px = XGetPixel( m, lx, ly );
			//(*(pb++)) = (px>>16) | (px&0xff00) | ((px&0xff)<<16);
			(*bp++) = px>>16;
			(*bp++) = px>>8;
			(*bp++) = px;
		}
	}
	XDestroyImage( m );
	return 0;
}



int main( int argc, char ** argv )
{
	int i;
	int showhelp = 0;
	char c;
	const char * outformat = "ppm";
	const char * outfile = 0;

	outputf = stdout;

	while( (c = getopt( argc, argv, "q:o:f:h?" )) != -1 )
	{
		switch( c )
		{
		case 'q':
			if( optarg )
			{
				quality = atoi( optarg );
			}
			else
			{
				fprintf( stderr, "-q expects parameter.\n" );
			}
			break;
		case 'o':
			outfile = optarg;
			break;
		case 'f':
			outformat = optarg;
			break;
		case '?': case 'h':
		default:
			showhelp = 1;
		}
	}

	if( showhelp || !outfile )
	{
		fprintf( stderr, "x11framegrab - grab x11 frames to files or stdio. Parameters:\n" );
		fprintf( stderr, "  -o [outfile, use - for stdout, REQUIRED field]\n" );
		fprintf( stderr, "  -f [format, default .ppm] available formats are:\n    " );
		for( i = 0; i < output_format_count; i++ )
			fprintf( stderr, "%s%c", output_formats[i], ((i+1)!=output_format_count)?',':'\n' );
		fprintf( stderr, "  -q [quality, if applicable, 1 = bad, 2 = ok, 3 = good]\n" );
		return -5;
	}
	if( CapScreen() )
	{
		fprintf( stderr, "Fatal error capturing screen.\n" );
		return -1;
	}

	if( strcmp( outfile, "-" ) == 0 )
	{
		outputf = stdout;
	}
	else
	{
		outputf = fopen( outfile, "wb" );
	}

	if( !outputf )
	{
		fprintf( stderr, "Error: Could not open file \"%s\" for writing.\n", outfile );
		return -8;
	}

	for( i = 0; i < output_format_count; i++ )
	{
		if( strcmp( outformat, output_formats[i] ) == 0 )
		{
			//Match
			return output_format_writer[i]( );
		}
	}

	fprintf( stderr, "Error: Could not find output format driver: %s\n", outformat );
	return -1;
}

/*

Copyright (c) 2016 <>< Charles Lohr

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
