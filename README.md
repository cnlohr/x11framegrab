# x11framegrab

Small (<32kB) static program for taking screenshots of x11 screens and saving/streaming as a variety of formats.

```
x11framegrab - grab x11 frames to files or stdio. Parameters:
  -o [outfile, use - for stdout, REQUIRED field]
  -f [format, default .ppm] available formats are:
    jpg,bmp,tga,png,ppm
  -q [quality, if applicable, 1 = bad, 2 = ok, 3 = good]
  -s [rescale quantity, # of times to divide by 2]
```

It does the jpg, png, tga, bmp statically.  It uses the single-file public domain library for that https://github.com/nothings/stb


