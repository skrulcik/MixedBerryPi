/** rgbstrip.c - RGB LED strip model
 *
 * Scott Krulcik 12/21/15
 *
 * Software layer to control RGB LED strip on raspberry pi
 */
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
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
    softPwmCreate(rs->rpin, 0, 100);
    rs->gpin = g;
    pinMode(rs->gpin, OUTPUT);
    softPwmCreate(rs->gpin, 0, 100);
    rs->bpin = b;
    pinMode(rs->bpin, OUTPUT);
    softPwmCreate(rs->bpin, 0, 100);
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
    softPwmWrite(rs->rpin, (rs->c.r * 100)/255);
    softPwmWrite(rs->gpin, (rs->c.g * 100)/255);
    softPwmWrite(rs->bpin, (rs->c.b * 100)/255);
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


