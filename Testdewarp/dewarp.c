

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <time.h>

#include "stb_image_write.h"
#include "stb_image.h"

typedef float DTYPE;

/* dewarps quad specified in "quad" from "in" into "out". */
void dewarp(int width, int height, unsigned char * in,  unsigned char * out, DTYPE * quad )
{
	DTYPE x1 = quad[0]; // top left
	DTYPE y1 = quad[1];
	DTYPE x2 = quad[2]; // top right
	DTYPE y2 = quad[3];
	DTYPE x3 = quad[4]; // lower right
	DTYPE y3 = quad[5];
	DTYPE x4 = quad[6]; // lower left
	DTYPE y4 = quad[7];
	
	unsigned char * dst = out;
	for(int h=0; h < height; h++)
	{
		DTYPE step = (DTYPE)h / (DTYPE)height;
	    DTYPE x_span_1 = (x4-x1) * step + x1; // left edge
		DTYPE y_span_1 = (y4-y1) * step + y1;
	    DTYPE x_span_2 = (x3-x2) * step + x2; // right edge
		DTYPE y_span_2 = (y3-y2) * step + y2;
		
		for (int w = 0; w < width; w++)
		{
			/* calculate source pixel location */
			DTYPE x = (x_span_2 - x_span_1) * w / width + x_span_1;
			DTYPE y = (y_span_2 - y_span_1) * w / width + y_span_1;
			*dst = in[(int)y*width + (int)x];
			dst++;
		}
	}
}


int main(void)
{

	int w, h, c;
	unsigned char * in = stbi_load("in.png", &w, &h, &c, 1);



	printf("w %d h %d c %d\n", w, h, c);

	unsigned char * out = malloc(w*h * sizeof(unsigned char));

	DTYPE quad[] = { 264,285, 732, 285, 881, 834, 155, 834 };
	int nr = 1000;

	clock_t tic = clock();
	for (int i = 0; i < nr; i++) {
		dewarp(w, h, in, out, quad);
	}

	clock_t toc = clock();
	double t = (double)(toc - tic) / CLOCKS_PER_SEC;
	printf("Elapsed: %f seconds for %d,  %f per dewarp\n", t, nr, t/nr );

	stbi_write_png("out.png", w, h, 1, out, w);


	free(in);
	free(out);
	return 0;
}