// Sachin Shah
// March 17, 2020
// Histogram

#ifndef _HISTOGRAM_H
#define _HISTOGRAM_H

#include "image.h"
#include <stdint.h>

uint32_t **create_histogram(void);
void *free_histogram(uint32_t **ptr);
void populate_histogram(uint32_t **histogram, Image *img);

uint32_t get_weight(uint32_t *histogram, uint32_t start, uint32_t end);
uint32_t balance(uint32_t *histogram);

void threshold(Image *img);
void colorize_3_classes(Image *img, uint32_t red, uint32_t green, uint32_t blue);
void colorize_2_classes(Image *img, uint32_t red, uint32_t green, uint32_t blue);
void colorize_rgb(Image *img, uint32_t red, uint32_t green, uint32_t blue);

#endif
