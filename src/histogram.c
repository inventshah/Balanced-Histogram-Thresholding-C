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
	uint32_t x, y;
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
			}
		}
	}
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
	colorize_rgb(img, red, green, blue);
	colorize_3_classes(img, red, green, blue);
	colorize_2_classes(img, red, green, blue);

	histogram = free_histogram(histogram);
}

void colorize_3_classes(Image *img, uint32_t red, uint32_t green, uint32_t blue)
{
	uint32_t x, y;
	png_bytep rgb;

	Image *output = create_image(img->width, img->height);

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			if (rgb != NULL)
			{
				if (rgb[0] < red && rgb[1] < green && rgb[2] < blue)
				{
					set_rgba(output, x, y, 0, 0, 0, 255);
				}
				else if (rgb[0] > red && rgb[1] > green && rgb[2] > blue)
				{
					set_rgba(output, x, y, 255, 255, 255, 255);
				}
				else
				{
					set_rgba(output, x, y, 120, 120, 120, 255);
				}
			}
		}
	}

	save_image(output, "output/three_classes.png");
	free_image(output);
}

void colorize_rgb(Image *img, uint32_t red, uint32_t green, uint32_t blue)
{
	uint32_t x, y;
	png_bytep rgb;
	uint8_t r, g, b;

	Image *output = create_image(img->width, img->height);

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			if (rgb != NULL)
			{
				r = (rgb[0] > red) ? 255 : 0;
				set_rgba(output, x, y, r, 0, 0, 255);
			}
		}
	}
	save_image(output, "output/red.png");

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			if (rgb != NULL)
			{
				g = (rgb[1] > green) ? 255 : 0;
				set_rgba(output, x, y, 0, g, 0, 255);
			}
		}
	}
	save_image(output, "output/green.png");

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			if (rgb != NULL)
			{
				b = (rgb[2] > blue) ? 255 : 0;
				set_rgba(output, x, y, 0, 0, b, 255);
			}
		}
	}
	save_image(output, "output/blue.png");

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			if (rgb != NULL)
			{
				r = (rgb[0] > red) ? 255 : 0;
				g = (rgb[1] > green) ? 255 : 0;
				b = (rgb[2] > blue) ? 255 : 0;
				set_rgba(output, x, y, r, g, b, 255);
			}
		}
	}
	save_image(output, "output/rgb.png");

	free_image(output);
}

void colorize_2_classes(Image *img, uint32_t red, uint32_t green, uint32_t blue)
{
	uint32_t x, y;
	png_bytep rgb;

	Image *output = create_image(img->width, img->height);

	for (x = 0; x < img->width; x++)
	{
		for (y = 0; y < img->height; y++)
		{
			rgb = get_rgb(img, x, y);
			if (rgb != NULL)
			{
				if (rgb[0] < red && rgb[1] < green && rgb[2] < blue)
				{
					set_rgba(output, x, y, 0, 0, 0, 255);
				}
				else
				{
					set_rgba(output, x, y, 255, 255, 255, 255);
				}
			}
		}
	}

	save_image(output, "output/two_classes.png");
	free_image(output);
}
