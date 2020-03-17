// Sachin Shah
// March 17, 2020
// Histogram

#include "histogram.h"
#include "image.h"
#include "utils.h"

#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t **create_histogram(void)
{
	int8_t i;
	uint32_t **ptr = (uint32_t **) calloc(3, sizeof(uint32_t *));
	check_null(ptr, "calloc failed to find space of a histogram in create_histogram");

	for (i = 0; i < 3; i++)
	{
		ptr[i] = (uint32_t *) calloc(256, sizeof(uint32_t));
		check_null(ptr, "calloc failed to find space of a histogram in create_histogram");
	}

	return ptr;
}

void *free_histogram(uint32_t **ptr)
{
	int8_t i;

	for (i = 0; i < 3; i++) free(ptr[i]);
	free(ptr);

	return NULL;
}

void populate_histogram(uint32_t **histogram, Image *img)
{
	uint32_t x, y, count;
	png_bytep rgb;

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			if (rgb != NULL)
			{
				histogram[0][rgb[0]]++;
				histogram[1][rgb[1]]++;
				histogram[2][rgb[2]]++;
				count++;
				//if (rgb[0] == 240) printf("rgb(%u %u %u) = h(%u %u %u)\n", rgb[0], rgb[1], rgb[2], histogram[0][rgb[0]], histogram[1][rgb[1]], histogram[2][rgb[2]]);
			}
		}
	}
	printf("count %u\n", count);
}

uint32_t get_weight(uint32_t *histogram, uint32_t start, uint32_t end)
{
	uint32_t i, weight = 0;

	for (i = start; i < end; i++) weight += histogram[i];

	return weight;
}

uint32_t balance(uint32_t *histogram)
{
	int32_t middle, start = 0, end = 255;
	uint32_t left_weight, right_weight;

	middle = (start + end) / 2;

	left_weight = get_weight(histogram, start, middle);
	right_weight = get_weight(histogram, middle, end + 1);
	printf("weight(%u, %u)\n", left_weight, right_weight);
	while (start <= end)
	{
		if (right_weight > left_weight)
		{
			right_weight -= histogram[end];
			end--;
			if ((start + end) / 2 < middle)
			{
				right_weight += histogram[middle];
				left_weight -= histogram[middle];
				middle--;
			}
		}
		else
		{
			left_weight -= histogram[start];
			start++;
			if ((start + end) / 2 >= middle)
			{
				middle++;
				left_weight += histogram[middle];
				right_weight -= histogram[middle];
			}
		}
	}

	return middle;
}

void threshold(Image *img)
{
	uint32_t red, green, blue;
	uint32_t **histogram = create_histogram();

	printf("Populating hisogram\n");
	populate_histogram(histogram, img);

	printf("Balancing red\n");
	red = balance(histogram[0]);

	printf("Balancing green\n");
	green = balance(histogram[1]);

	printf("Balancing blue\n");
	blue = balance(histogram[2]);

	printf("Threshold = rgb(%u, %u, %u)\n", red, green, blue);

	printf("Colorizing\n");
	colorize(img, red, green, blue);

	histogram = free_histogram(histogram);
}

void colorize(Image *img, uint32_t red, uint32_t green, uint32_t blue)
{
	uint32_t x, y;
	png_bytep rgb;

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			if (rgb != NULL)
			{
				if (rgb[0] < red && rgb[1] < green && rgb[2] < blue)
				{
					set_rgb(img, x, y, 0, 0, 0);
				}
				else
				{
					set_rgb(img, x, y, 255, 255, 255);
				}
			}
		}
	}
}
