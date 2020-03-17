// Sachin Shah
// March 12, 2020

#include "image.h"
#include "histogram.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	Image *img = NULL;
	char *source, *save;
	uint8_t bandwidth;
	uint32_t gens = 50;

	if (argc < 2) error("Too few command line arguments. Need [source filename]");

	img = open_image(argv[1]);

	threshold(img);

	img = free_image(img);

	return 0;
}