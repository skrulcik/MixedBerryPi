/** rgbstrip.h - RGB LED strip model
 *
 * Scott Krulcik 12/21/15
 *
 * Software layer to control RGB LED strip on raspberry pi
 */
#include "color.h"

struct rgbstrip_data;
typedef struct rgbstrip_data rgbstrip;

void rgb_setup();
rgbstrip *rgb_init(color *c, int r, int g, int b);
void rgb_free(rgbstrip *rs);

void rgb_set(rgbstrip *rs, color *c);
color *rgb_get(rgbstrip *rs);

