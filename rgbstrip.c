/** rgbstrip.c - RGB LED strip model
 *
 * Scott Krulcik 12/21/15
 *
 * Software layer to control RGB LED strip on raspberry pi
 */
#include <stdio.h>
#include "rgbstrip.h"

struct rgbstrip_data {
    color c;
};

void rgb_print(rgbstrip *rs);

void rgb_set(rgbstrip *rs, color *c) {
    rs->c.r = c->r;
    rs->c.g = c->g;
    rs->c.b = c->b;
    rs->c.a = c->a;
#if DEBUG
    rgb_print(rs);
#endif
}

color *rgb_get(rgbstrip *rs) {
    return colorcpy(&rs->c);
}

void rgb_print(rgbstrip *rs) {
    printf("-----\n");
    printf("R %3d\n", (unsigned int)rs->c.r);
    printf("G %3d\n", (unsigned int)rs->c.g);
    printf("B %3d\n", (unsigned int)rs->c.b);
    printf("-----\n");
}
