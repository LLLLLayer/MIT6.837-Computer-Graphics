#include <stdio.h>
#include <assert.h>
#include "scene_parser.h"
#include "image.h"
#include "matrix.h"
#include "camera.h" 
#include "material.h"
#include "hit.h"
#include "object3d.h"
#include "ray.h"
#include "vectors.h"
using namespace std;

int main(int argc, char *argv[]){
	char *input_file = NULL;
	int width = 100;
	int height = 100;
	char *output_file = NULL;
	float depth_min = 0;
	float depth_max = 1;
	char *depth_file = NULL;
	for (int i = 1; i < argc; i++){
		if (!strcmp(argv[i], "-input")){
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")){
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")){
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")){
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	assert(input_file != NULL);
	SceneParser* sceneParser = new SceneParser(input_file);
	int num_materials = sceneParser->getNumMaterials();
	Camera* camera = sceneParser->getCamera();
	Group* group = sceneParser->getGroup();
	Vec3f background_color = sceneParser->getBackgroundColor();
	int resolution = width < height ? width : height;
	Image* image = new Image(resolution, resolution);
	image->SetAllPixels(background_color);
	Image* depth_image = new Image(resolution, resolution);
	depth_image->SetAllPixels(Vec3f(0.0, 0.0, 0.0));

	float precalc = depth_max - depth_min;
	if (precalc == 0){
		precalc = 1.0f;
	}
	
	for (int j = 0; j < resolution; j++){
		for (int i = 0; i < resolution; i++){
			float x = (float)i * 2.0f / resolution - 1.0f;
			float y = (float)j * 2.0f / resolution - 1.0f;
			Ray ray = camera->generateRay(Vec2f(x, y));
			Hit* hit = new Hit(INT_MAX, sceneParser->getMaterial(0));
			float tmin = -INT_MAX;
			if (group->intersect(ray, *hit, tmin)){
				image->SetPixel(i, j, hit->getMaterial()->getDiffuseColor());
				float t = hit->getT();
				if (t < depth_min){
					t = depth_min;
				}
				if (t > depth_max){
					t = depth_max;
				}			
				float volumn = (depth_max - t) / precalc;
				depth_image->SetPixel(i, j, Vec3f(volumn, volumn, volumn));	
			}
		}
	}

	image->SaveTGA(output_file);
	depth_image->SaveTGA(depth_file);
	return 0;
}

