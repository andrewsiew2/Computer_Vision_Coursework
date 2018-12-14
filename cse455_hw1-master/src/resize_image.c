#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // YOUR CODE HERE
    int w;
    int h;
    w = floor(x);
    h = floor(y);
    if (w < 0) {
        w = 0;
    }
    if (h < 0) {
        h = 0;
    }    

    return im.data[(int) (w + im.w * h + im.w * im.h * c)];
}

image nn_resize(image im, int w, int h)
{
    image ret = make_image(w, h, im.c);
    // YOUR CODE HERE
    int i;
    int j;
    int k;

    float scaleW = ((float) im.w) / ((float) w);
    float scaleH = ((float) im.h) / ((float) h);
    for (i = 0; i < im.c; i++) {
        for (j = 0; j < h; j++) {
            for (k = 0; k < w; k++) {

                ret.data[h*w*i + j*w + k] = nn_interpolate(im, (k + 0.5) * scaleW, (j+0.5) * scaleH, i);
            }
            
        }
        
    }
    return ret;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // YOUR CODE HERE
    int upperBoundX = ceilf(x);
    int lowerBoundX = floorf(x);
    int upperBoundY = ceilf(y);
    int lowerBoundY = floorf(y);

    //printf("error\n");
    float V1 = get_pixel(im, lowerBoundX, lowerBoundY, c);
    float V2 = get_pixel(im, upperBoundX, lowerBoundY, c);
    float V3 = get_pixel(im, lowerBoundX, upperBoundY, c);
    float V4 = get_pixel(im, upperBoundX, upperBoundY, c);
    float q1 = V1 * (upperBoundX - x) + V2 * (x - lowerBoundX);
    float q2 = V3 * (upperBoundX - x) + V4 * (x - lowerBoundX);
    float q = q1 * (y - lowerBoundY) + q2 * (upperBoundY - y);



    return V1 * (upperBoundX - x) * (upperBoundY - y)
    + V4 * (x - lowerBoundX) * (y - lowerBoundY) 
    + V3 * (upperBoundX - x) * (y - lowerBoundY)
    + V2 * (x - lowerBoundX) * (upperBoundY - y);
}

image bilinear_resize(image im, int w, int h)
{
    image ret = make_image(w, h, im.c);
    // YOUR CODE HERE
    int i;
    int j;
    int k;
    float scaleW = ((float) im.w) / ((float) w);
    float scaleH = ((float) im.h) / ((float) h);
    // printf("h = %i\n", h);
    // printf("w = %i\n", w);
    float b_w = -0.5 + 0.5*scaleW;
    float b_h = -0.5 + 0.5*scaleH;
    for (i = 0; i < im.c; i++) {
        for (j = 0; j < h; j++) {
            for (k = 0; k < w; k++) {
                //printf("%i\n", h*w*i + j*w + k);
                ret.data[h*w*i + j*w + k] = bilinear_interpolate(im, 
                    scaleW * k + b_w, scaleH * j + b_h, i);
            }
            
        }
        
    }
    return ret;
}

