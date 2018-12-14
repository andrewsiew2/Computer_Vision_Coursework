#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    if (c >= im.c) {
        c = im.c - 1 ;
    } 
    if (y >= im.h) {
        y = im.h - 1;
    }
    if (x >= im.w) {
        x = im.w - 1;
    }
    if (c < 0) {
        c = 0;
    } 
    if (y < 0) {
        y = 0;
    }
    if (x < 0) {
        x = 0;
    }
    return im.data[x + im.w * y + im.w * im.h * c];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    im.data[x + im.w * y + im.h * im.w * c] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    int i;
    for (i = 0; i < im.w * im.h * im.c; i++) {
        copy.data[i] = im.data[i];
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    int i;
    for (i = 0; i < im.w * im.h; i++) {
        int red = im.data[i];
        int green = im.data[im.w * im.h + i];
        int blue = im.data[im.w * im.h * 2 + i];
        gray.data[i] = 0.299 * red + 0.587 * green + 0.114 * blue;
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    int i;
    for (i = 0; i < im.w * im.h; i++) {
        im.data[im.w * im.h * c + i] = im.data[im.w * im.h * c + i] + v; 
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    int i;
    for (i = 0; i < im.w * im.h * im.c; i++) {
        if (im.data[i] > 1.0) {
            im.data[i] = 1.0;
        }
        if (im.data[i] < 0.0) {
            im.data[i] = 0.0;
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    int i;
    for (i = 0; i < im.w * im.h; i++) {
        float red = im.data[i];
        float green = im.data[im.w * im.h + i];
        float blue = im.data[2 * im.w * im.h + i];
        float value = three_way_max(red, green, blue);
        float min = three_way_min(red, green, blue);
        float c = value - min;
        float sat;
        if (value == 0.0) {
            sat = 0.0;
        }else {
            sat =  c / value;
        }
        float hue;
        float huePrime;
        if (c == 0.0) {
            hue = 0.0;
        } else {
            if (value == red) {
                huePrime = (green - blue) / c;
            } else if (value == green) {
                huePrime = (blue - red) / c + 2.0;
            } else {
                huePrime = (red - green) / c + 4.0;
            }

            if (huePrime < 0.0) {
                hue = huePrime / 6.0 + 1.0;
            } else {
                hue = huePrime / 6.0;
            }
        }

        im.data[i] = hue;
        im.data[im.w * im.h + i] = sat;
        im.data[2 * im.w * im.h + i] = value;
    } 
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    int i;
    for (i = 0; i < im.w * im.h; i++) {
        float hue = im.data[i];
        float sat = im.data[im.w * im.h + i];
        float v = im.data[2 * im.w * im.h + i];
        float huePrime;
        float c = sat * v;
        float min = v - c;
        float colorAngle = hue * 6.0;
        float value = hue * 6;
        if (value - 4 > 0) {
            value -= 4;
        } else if (value - 2 > 0) {
            value -= 2;
        }
        value -= 1.0;
        value = fabs(value);
        value = 1.0 - value;
        value *= c;
        if (colorAngle >= 0.0 && colorAngle < 1.0) {
            im.data[i] = c + min;
            im.data[im.w * im.h + i] = value + min;
            im.data[2 * im.w * im.h + i] = min;
        } else if (colorAngle >= 1.0 && colorAngle < 2.0) {
            im.data[i] = value + min;
            im.data[im.w * im.h + i] = c + min;
            im.data[2 * im.w * im.h + i] = min;
        } else if (colorAngle >= 2.0 && colorAngle < 3.0) {
            im.data[i] = min;
            im.data[im.w * im.h + i] = c + min;
            im.data[2 * im.w * im.h + i] = value + min;
        } else if (colorAngle >= 3.0 && colorAngle < 4.0) {
            im.data[i] = min;
            im.data[im.w * im.h + i] = value + min;
            im.data[2 * im.w * im.h + i] = c + min;
        } else if (colorAngle >= 4.0 && colorAngle < 5.0) {
            im.data[i] = value + min;
            im.data[im.w * im.h + i] = min;
            im.data[2 * im.w * im.h + i] = c + min;
        } else if (colorAngle >= 5.0 && colorAngle < 6.0) {
            im.data[i] = c + min;
            im.data[im.w * im.h + i] = min;
            im.data[2 * im.w * im.h + i] = value + min;
        } else {
            im.data[i] = min;
            im.data[im.w * im.h + i] = min;
            im.data[2 * im.w * im.h + i] = min;
        }
    }
}

void scale_image(image im, int c, float v)
{
    // TODO Fill this in
    int i;
    for (i = 0; i < im.w * im.h; i++) {
        im.data[im.w * im.h * c + i] = im.data[im.w * im.h * c + i] *= v; 
    }
}
