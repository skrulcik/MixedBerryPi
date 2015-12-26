/** color.c - color abstraction for mixedberrypi
 *
 * Scott Krulcik 12/21/15
 *
 */

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

