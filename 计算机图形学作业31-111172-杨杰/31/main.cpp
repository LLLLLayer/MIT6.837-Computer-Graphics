//
// originally implemented by Justin Legakis
//
#include "matrix.h"
#include "vectors.h"
#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include<vector>

float get_rand(int size) {
	float b;
	b = (rand() % size)*(1.0000f/size);
	return b;
}
int main(int argc,  char** argv)
{
	char *input_file = NULL;
	int width = 100;
	int height = 100;
	char *output_file = NULL;
	float depth_min = 0;
	float depth_max = 1;
	char *depth_file = NULL;
	int points = 0;
	int size = 0;
	int iters = 0;
	// sample command line:
	// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			const char * p = argv[i];
			input_file = argv[i];
		}

		else if (!strcmp(argv[i], "-points")) {
			i++; assert(i < argc);
			points = atoi(argv[i]);

		}
		else if (!strcmp(argv[i], "-iters")) {
			i++; assert(i < argc);
			iters = atoi(argv[i]);

		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			size = atoi(argv[i]);
			
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = (argv[i]);
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}

	}
	FILE* f = fopen(input_file, "rb");
	if (f == 0)
	{
		printf("can't open input_file");
		return false;
	}
	int n=0;
	fscanf(f, "%d", &n);
	vector<Matrix> temp;
	vector<float> ca;
	while (n--) {
			float s;
        	fscanf(f, "%f", &s);
			ca.push_back(s);
			Matrix t;
			t.Read3x3(f);
			temp.push_back(t);
	}
	//vector<pair<float,vector<vector<float>>>> tp;
	//while (n--) {
	//	vector<vector<float>> temp;
	//	float a[3];
	//	float s;
	//	fscanf(f, "%f", &s);
	//	for (int i = 0; i < 3; i++) {
	//		vector<float> ts;
	//		fscanf(f, "%f %f %f", &a[0], &a[1], &a[2], &a[3]);
	//		for (int j = 0;j < 3; j++) {
	//			ts.push_back(a[j]);
	//		}
	//		temp.push_back(ts);
	//	}
	//
	//	tp.push_back(pair<float, vector<vector<float>>>(s,temp));
	//}

	Image image(size, size);
	Vec3f color(255, 255,255);
	image.SetAllPixels(color);
	//image.SaveTGA("mmp.tga");
	for (int i = 1; i < ca.size(); i++) {
		ca[i] = ca[i] + ca[i - 1];
	}
	for (int i = 0; i < points; i++) {
		float x = get_rand(size);
		float y = get_rand(size);
		Vec2f point(x, y);
		Matrix t_point;
		//t_point.Transform(point);
		for (int j = 0; j < iters; j++) {
			
			float f = rand() % size*(1.00000f/size);
			int ch;
			for (int k = 0; k < ca.size(); k++) {
				if (f < ca[k])
				{
					ch = k; break;
				}
			}
			 temp[ch].Transform(point);
			
		}
		Vec3f cl(0, 0, 255);
		image.SetPixel(point.x() * size, point.y() *size, cl);

	}
	image.SaveTGA(output_file);
	//system("pause");
	return 0;
	// Read each line.
	//fscanf(f, "%d", &n);
}