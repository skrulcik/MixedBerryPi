/** color.h - color abstraction for mixedberrypi
 *
 * Scott Krulcik 12/21/15
 *
 */

typedef struct color_data {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} color;


color *color_init() 
color *colorcpy(color *og);


