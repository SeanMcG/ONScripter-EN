#ifndef _ONSRECTADAPTER_H
#define _ONSRECTADAPTER_H

#include <SDL.h>
#include "ONSrect.h"

class ONSrectAdapter : public ONSrect {
    public:
        ONSrectAdapter(SDL_Rect &rect) {
            x = rect.x;
            y = rect.y;
            w = rect.w;
            h = rect.h;
        }
};

#endif