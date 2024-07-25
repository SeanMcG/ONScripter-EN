#ifndef _PIXELFORMAT_H
#define _PIXELFORMAT_H

namespace ONSPixel {
  typedef struct PixelFormat {
    unsigned char Rshift;
    unsigned char Gshift;
    unsigned char Bshift;
    unsigned long Rmask;
    unsigned long Gmask;
    unsigned long Bmask;
  } PixelFormat;
}

#endif
