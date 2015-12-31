/** rgbstrip.c - RGB LED strip model
 *
 * Scott Krulcik 12/21/15
 *
 * Software layer to control RGB LED strip on raspberry pi
 */
#include <stdio.h>
#include <wiringPi.h>
#include "csapp.h"
#include "rgbstrip.h"

struct rgbstrip_data {
    color c;
    int rpin;
    int gpin;
    int bpin;
};

void rgb_print(rgbstrip *rs);
void rgb_refreshpins(rgbstrip *rs);

void rgb_setup() {
    wiringPiSetup();
}

rgbstrip *rgb_init(color *c, int r, int g, int b) {
    rgbstrip *rs = (rgbstrip *)Malloc(sizeof(rgbstrip));
    rs->rpin = r;
    pinMode(rs->rpin, OUTPUT);
    rs->gpin = g;
    pinMode(rs->gpin, OUTPUT);
    rs->bpin = b;
    pinMode(rs->bpin, OUTPUT);
    rgb_set(rs, c);
    return rs;
}
void rgb_free(rgbstrip *rs) {
    free(rs);
}

void rgb_set(rgbstrip *rs, color *c) {
    rs->c.r = c->r;
    rs->c.g = c->g;
    rs->c.b = c->b;
    rs->c.a = c->a;
    rgb_refreshpins(rs);
#if DEBUG
    rgb_print(rs);
#endif
}

void rgb_refreshpins(rgbstrip *rs) {
    if (rs->c.r > 127)
        digitalWrite(rs->rpin, HIGH);
    else
        digitalWrite(rs->rpin, LOW);
    printf("%d: %s\n", rs->rpin, (rs->c.r > 127) ? "HIGH":"LOW");
    if (rs->c.g > 127)
        digitalWrite(rs->gpin, HIGH);
    else
        digitalWrite(rs->gpin, LOW);
    if (rs->c.b > 127)
        digitalWrite(rs->bpin, HIGH);
    else
        digitalWrite(rs->bpin, LOW);
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


