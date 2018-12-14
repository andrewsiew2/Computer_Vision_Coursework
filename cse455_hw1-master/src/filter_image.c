#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846


void l1_normalize(image im)
{
  // YOUR CODE HERE
  int i;
  float total = 0.0;
  for (i = 0; i < im.w * im.h * im.c; i++){
    total += im.data[i];
  }
  for (i = 0; i < im.w * im.h * im.c; i++){
    im.data[i] /= total;
  }
}

image make_box_filter(int w)
{
  image ret = make_image(w,w,1);
  int i;
  for (i = 0; i < w * w; i++){
    ret.data[i] = 1.0/(w * w);
  }
  // YOUR CODE HERE
  return ret;
}

float calculate_center_value(image filter, image im,
              int indexX, int indexY, int indexChan, int filterChan)
{
  int difference = floor(((float) filter.w) / 2.0);
  int i;
  int j;

  float ret = 0.0;
  int count = 0;
  for (i = indexY - difference; i <= indexY + difference; i++) {
    for (j = indexX - difference; j <= indexX + difference; j++) {

      ret += filter.data[count] * 
              get_pixel(im, j, i, indexChan);
      count++;
    }
  }

  return ret;
}

image convolve_image(image im, image filter, int preserve)
{
  image ret;
  // YOUR CODE HERE
  
  assert(filter.c == im.c || filter.c == 1);
  // Case 1:
  // preserve = 1, filter.c = 1
  // retain number of channels
  // apply filter to every channel
  if (preserve == 1 && filter.c == 1) {
    ret = make_image(im.w,im.h,im.c);
    int i;
    int j;
    int k;
    for (i = 0; i < im.c; i++) {
      for (j = 0; j < im.h; j++) {
        for (k = 0; k < im.w; k++) {
          int index = k + j * im.w + i * im.h * im.w;
          ret.data[index] += calculate_center_value(filter, im, 
                            k, j, i, 0);
        }
      }
    }
  }

  // Case 2:
  // preserve != 1 , filter.c = 1
  // have 1 channel
  else if (preserve != 1 && filter.c == 1) {
    int i;
    int j;
    int k;
    ret = make_image(im.w,im.h,1);
    for (j = 0; j < im.h; j++) {
      for (k = 0; k < im.w; k++) {
        for (i = 0; i < im.c; i++) {
          float total = 0.0;

          int index = k + j * im.w + i * im.h * im.w;
          ret.data[k + j * im.w] += calculate_center_value(filter, im,k , j, i, 0);
        }
      }
    }
  }

  // Case 3
  // preserve = 1
  // same number of channels
  else if (preserve == 1) {
    ret = make_image(im.w,im.h,im.c);
    int i;
    int j;
    int k;
    for (i = 0; i < im.c; i++) {
      for (j = 0; j < im.h; j++) {
        for (k = 0; k < im.w; k++) {
          int index = k + j * im.w + i * im.h * im.w;
          ret.data[index] += calculate_center_value(filter, im, 
                            k,j , i, i);
        }
      }
    }
  }
  // Case 4:
  // im.c == filter.c 
  else {
    ret = make_image(im.w,im.h,1);

    int i;
    int j;
    int k;
    for (i = 0; i < im.c; i++) {
      for (j = 0; j < im.h; j++) {
        for (k = 0; k < im.w; k++) {
          int index = k + j * im.w + i * im.h * im.w;
          ret.data[k + j * im.w] += calculate_center_value(filter, im,k,j, i, i);
        }
      }
    }
  }



  return ret;
}

image make_highpass_filter()
{
  image ret = make_image(3,3,1);
  // YOUR CODE HERE
  ret.data[0] = 0;
  ret.data[1] = -1;
  ret.data[2] = 0;
  ret.data[3] = -1;
  ret.data[4] = 4;
  ret.data[5] = -1;
  ret.data[6] = 0;
  ret.data[7] = -1;
  ret.data[8] = 0;
  return ret;
}

image make_sharpen_filter()
{
  image ret = make_image(3,3,1);
  // YOUR CODE HERE
  ret.data[0] = 0;
  ret.data[1] = -1;
  ret.data[2] = 0;
  ret.data[3] = -1;
  ret.data[4] = 5;
  ret.data[5] = -1;
  ret.data[6] = 0;
  ret.data[7] = -1;
  ret.data[8] = 0;
  return ret;
}

image make_emboss_filter()
{
  image ret = make_image(3,3,1);
  ret.data[0] = -2;
  ret.data[1] = -1;
  ret.data[2] = 0;
  ret.data[3] = -1;
  ret.data[4] = 1;
  ret.data[5] = 1;
  ret.data[6] = 0;
  ret.data[7] = 1;
  ret.data[8] = 2;
  // YOUR CODE HERE
  return ret;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: Run gaussian,sharpen,emboss with preserve and the rest don't need to have preserve
// Reason is that the stated filters still have colors unlike the rest. 

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: gaussian,sharpen,highpass and emboss need post processing. 
// Reason is we add numbers when we convolve the image so we can add number until the go out 
// of bounds. The pixel will still have a value but of a value that is overflown.

image make_gaussian_filter(float sigma)
{
  float width = sigma * 6;
  if (((int) ceilf(width)) % 2 == 0) {
    width = ceilf(width) + 1;
  } else {
    width = ceilf(width);
  }
  image ret = make_image((int) width,(int) width,1);
  int y;
  int x;
  int index = 0;
  for (y = (width / 2) * (-1);y <= width / 2; y++) {
    for (x = (width / 2) * (-1); x <= width / 2; x++) {
      float value = 0.0;
      float r = sqrt(x*x + y*y);
      value += (2.0 * M_PI * sigma * sigma);
      value = exp((-1) * (r*r) / (2.0 * sigma * sigma)) / value;
      ret.data[index] = value;
      index++;
    }
  }
  // YOUR CODE HERE
  l1_normalize(ret);
  return ret;
}

image add_image(image a, image b)
{
  image ret = make_image(a.w,a.h,a.c);
  // YOUR CODE HERE
  assert(a.w == b.w);
  assert(a.h == b.h);
  assert(a.c == b.c);

  int i;
  for (i = 0; i < a.w*a.h*a.c; i++) {
    ret.data[i] = a.data[i] + b.data[i];
  }
  return ret;
}

image sub_image(image a, image b)
{
  image ret = make_image(a.w,a.h,a.c);
  // YOUR CODE HERE
  assert(a.w == b.w);
  assert(a.h == b.h);
  assert(a.c == b.c);
  int i;
  for (i = 0; i < a.w*a.h*a.c; i++) {
    ret.data[i] = a.data[i] - b.data[i];
  }
  return ret;
}

image make_gx_filter()
{
  image ret = make_image(3,3,1);
  // YOUR CODE HERE
  ret.data[0] = -1;
  ret.data[1] = 0;
  ret.data[2] = 1;
  ret.data[3] = -2;
  ret.data[4] = 0;
  ret.data[5] = 2;
  ret.data[6] = -1;
  ret.data[7] = 0;
  ret.data[8] = 1;
  return ret;
}

image make_gy_filter()
{
  image ret = make_image(3,3,1);
  // YOUR CODE HERE
  ret.data[0] = -1;
  ret.data[1] = -2;
  ret.data[2] = -1;
  ret.data[3] = 0;
  ret.data[4] = 0;
  ret.data[5] = 0;
  ret.data[6] = 1;
  ret.data[7] = 2;
  ret.data[8] = 1;
  return ret;
}

void feature_normalize(image im)
{
  // YOUR CODE HERE
  int i;
  float max = im.data[0];
  float min = im.data[0];
  for (i = 0; i < im.w * im.h * im.c; i++){
    if (im.data[i] > max) {
      max = im.data[i];
    }
    if (im.data[i] < min) {
      min = im.data[i];
    }
  }
  float range = max - min;
  if (range == 0.0) {
    for (i = 0; i < im.w * im.h * im.c; i++){
      im.data[i] = 0.0;
    }
  } else {
    for (i = 0; i < im.w * im.h * im.c; i++){
      im.data[i] = (im.data[i] - min) / range;
    }
    
  }
}

image *sobel_image(image im)
{
  image *ret = calloc(2, sizeof(image));
  // YOUR CODE HERE
  image ret_image = make_image(im.w, im.h, 1);
  image ret_image2 = make_image(im.w, im.h, 1);
  ret[0] = ret_image;
  ret[1] = ret_image2;
  image gx_filter = make_gx_filter();
  image gy_filter = make_gy_filter();
  image image_gx = convolve_image(im, gx_filter, 0);
  image image_gy = convolve_image(im, gy_filter, 0);
  int i;
  for (i = 0; i < im.w * im.h; i++) {
    ret_image.data[i] = sqrt(image_gx.data[i]*image_gx.data[i] +
                       image_gy.data[i]*image_gy.data[i]);
    ret_image2.data[i] = atan2(image_gy.data[i],image_gx.data[i]);
  }


  return ret;
}

image colorize_sobel(image im)
{
  assert(im.c == 3);
  image ret1 = make_image(im.w,im.h,im.c);
  // YOUR CODE HERE
  image *ret = calloc(2, sizeof(image));
  // YOUR CODE HERE
  image ret_image = make_image(im.w, im.h, 1);
  image ret_image2 = make_image(im.w, im.h, 1);
  ret[0] = ret_image;
  ret[1] = ret_image2;
  image gx_filter = make_gx_filter();
  image gy_filter = make_gy_filter();
  image image_gx = convolve_image(im, gx_filter, 1);
  image image_gy = convolve_image(im, gy_filter, 1);
  int i;
  for (i = 0; i < im.w * im.h; i++) {
    ret_image.data[i] = sqrt(image_gx.data[i]*image_gx.data[i] +
                       image_gy.data[i]*image_gy.data[i]);
    ret_image2.data[i] = atan(image_gy.data[i] / image_gx.data[i]);
    float hue = ret_image.data[i];
    float sat = ret_image.data[i];
    float v = ret_image.data[i];
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
        ret1.data[i] = c + min;
        ret1.data[im.w * im.h + i] = value + min;
        ret1.data[2 * im.w * im.h + i] = min;
    } else if (colorAngle >= 1.0 && colorAngle < 2.0) {
        ret1.data[i] = value + min;
        ret1.data[im.w * im.h + i] = c + min;
        ret1.data[2 * im.w * im.h + i] = min;
    } else if (colorAngle >= 2.0 && colorAngle < 3.0) {
        ret1.data[i] = min;
        ret1.data[im.w * im.h + i] = c + min;
        ret1.data[2 * im.w * im.h + i] = value + min;
    } else if (colorAngle >= 3.0 && colorAngle < 4.0) {
        ret1.data[i] = min;
        ret1.data[im.w * im.h + i] = value + min;
        ret1.data[2 * im.w * im.h + i] = c + min;
    } else if (colorAngle >= 4.0 && colorAngle < 5.0) {
        ret1.data[i] = value + min;
        ret1.data[im.w * im.h + i] = min;
        ret1.data[2 * im.w * im.h + i] = c + min;
    } else if (colorAngle >= 5.0 && colorAngle < 6.0) {
        ret1.data[i] = c + min;
        ret1.data[im.w * im.h + i] = min;
        ret1.data[2 * im.w * im.h + i] = value + min;
    } else {
        ret1.data[i] = min;
        ret1.data[im.w * im.h + i] = min;
        ret1.data[2 * im.w * im.h + i] = min;
    }
  }
  return ret1;
}

image number_bilateral_filter(image im, image gafilter, int x, int y, int c, int width)
{
  int difference = (int) floorf(((float) width) / 2.0);
  int i;
  int j;
  float value = 0;
  float total = 0;
  float and = 0;
  int  count = 0;

  image ret = make_image(width, width, 1);
  for (i = y - difference; i <= y + difference; i++) {
    for (j = x - difference; j <= x + difference; j++) {
      float sigma = 3; // This value is editable
      float center = get_pixel(im,x,y,c);
      float gaValue = gafilter.data[count];
      float gbValue = exp(-1 * fabs(center - get_pixel(im, j, i, c))
                    / (2*sigma*sigma)) / (sqrt(2*M_PI)*sigma);
      ret.data[count] = gaValue * gbValue;
      total += ret.data[count];
      count++;
    }
  }
  for (i = 0; i < count; i++) {
    ret.data[i] /= total;
  }
  return ret;
}

image bilateral_filter(image im) 
{
  // YOUR CODE HERE
  float sigma = 1.3; // This value is editable
  float width = sigma * 6;
  if (((int) ceilf(width)) % 2 == 0) {
    width = ceilf(width) + 1;
  } else {
    width = ceilf(width);
  }
  image g1 = make_gaussian_filter(sigma);
  image g2 = make_image(width,width,1);
  image ret = make_image(im.w,im.h,im.c);
  
  int i;
  int j;
  int k;
  int l;
  int m;
  int count = 0;
  int diff = (int) floorf(((float) width) / 2.0);  
  for (i = 0; i < im.c; i++) {
    for (j = 0; j < im.h; j++) {
      for (k = 0; k < im.w; k++) {
        // create filter then do convolution
        float value = 0;
        image filter = number_bilateral_filter(im,g1,k,j,i, width);
        for (l = j - diff; l <= j + diff ; l++) {
          for (m = k - diff; m <= k + diff; m++) {
            value +=  (filter.data[count]) * (get_pixel(im, m, l, i));

            count++;
          }
        }
        count = 0;
        set_pixel(ret,k,j,i,value);

      }
    }
  }

  return ret;

}

void create_ronbledore() {
  image ron = load_image("data/ron.png");
  image dumbledore = load_image("data/dumbledore.png");
  image lowfre = convolve_image(dumbledore, make_gaussian_filter(3), 1);

  image high_freq = sub_image(ron, convolve_image(ron, make_gaussian_filter(2), 1));

  // 2.7
  image reconstruct = add_image(lowfre , high_freq);
  clamp_image(reconstruct);
  save_image(reconstruct, "output/ronbledore.jpg");
  free_image(ron);
  free_image(dumbledore);
  free_image(lowfre);
  free_image(high_freq);
  free_image(reconstruct);
}
