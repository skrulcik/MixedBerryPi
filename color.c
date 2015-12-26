/** color.c - color abstraction for mixedberrypi
 *
 * Scott Krulcik 12/21/15
 *
 */
#include "color.h"
#include "csapp.h"

#define RCOMP(x) (unsigned char)(((x) >> 24) & 0xFF)
#define GCOMP(x) (unsigned char)(((x) >> 16) & 0xFF)
#define BCOMP(x) (unsigned char)(((x) >> 8) & 0xFF)
#define ACOMP(x) (unsigned char)((x) & 0xFF)

color *color_init() {
    color *newc = (color *)Malloc(sizeof(color));
    return newc;
}

color *colorcpy(color *og) {
    color *newc = color_init();
    newc->r = og->r;
    newc->g = og->g;
    newc->b = og->b;
    newc->a = og->a;
    return newc;
}

void color_setint(color *c, int data) {
    c->r = RCOMP(data);
    c->g = GCOMP(data);
    c->b = BCOMP(data);
    c->a = ACOMP(data);
}

