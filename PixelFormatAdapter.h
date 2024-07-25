#ifndef _PIXELFORMATADAPTER_H
#define _PIXELFORMATADAPTER_H

namespace ONSPixel {

class PixelFormatAdapter : public PixelFormat {
  public:
    PixelFormatAdapter(SDL_PixelFormat &fmt) {
      Rmask = fmt.Rmask;
      Gmask = fmt.Gmask;
      Bmask = fmt.Bmask;
      Rshift = fmt.Rshift;
      Gshift = fmt.Gshift;
      Bshift = fmt.Bshift;
    }
};

}

#endif
