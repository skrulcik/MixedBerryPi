/** rgbstrip.c - RGB LED strip model
 *
 * Scott Krulcik 12/21/15
 *
 * Software layer to control RGB LED strip on raspberry pi
 */
#include "rgbstrip.h"

struct rgbstrip_data {
    color *color;

};

void rgb_set(rgbstrip *rs, color *c) {
    rs->color->r = c->r;
    rs->color->g = c->g;
    rs->color->b = c->b;
    rs->color->a = c->a;
}



color *rgb_get(rgbstrip *rs) {
    return colorcpy(rs->color);
}

