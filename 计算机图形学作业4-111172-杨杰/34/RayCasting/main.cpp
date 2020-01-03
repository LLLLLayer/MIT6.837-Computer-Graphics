
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
#include "light.h"
#include "raytracer.h"
#include "glCanvas.h"
#include <math.h>
using namespace std;

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool shade_back = false;
int bounce_limit = 0;
bool cast_shadow = false;
float cut_off_ray_weight = 0.0f;
int resolution = 0;
SceneParser* sceneParser;

void RenderScene() 
{


}

void traceRayFunction(float x, float y)
{
	x = x * 2.0f - 1.0f;
	y = y * 2.0f - 1.0f;
	Ray ray = sceneParser->getCamera()->generateRay(Vec2f(x, y));
	Hit h(INT_MAX, NULL, Vec3f(0.0f, 0.0f, 1.0f));
	float tmin = 0.1f;
	RayTracer rt(sceneParser, bounce_limit, cut_off_ray_weight, cast_shadow);
	Vec3f pixel = rt.traceRay(ray, tmin, 0, 1.0, 1.0, h, shade_back, true);
}



int main(int argc, char *argv[])
{
	// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-input")) 
		{
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) 
		{
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) 
		{
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth"))
		{
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals"))
		{
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back"))
		{
			shade_back = true;
		}
		else if (!strcmp(argv[i], "-bounces")) {
			i++;
			assert(i < argc);
			bounce_limit = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight")) {
			i++;
			assert(i < argc);
			cut_off_ray_weight = (float)(atof(argv[i]));
		}
		else if (!strcmp(argv[i], "-shadows")) {
			cast_shadow = true;
		}
		else 
		{
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	assert(input_file != NULL);
	sceneParser = new SceneParser(input_file);
	int num_materials = sceneParser->getNumMaterials();
	int num_lights = sceneParser->getNumLights();

	Camera* camera = sceneParser->getCamera();
	Group* group = sceneParser->getGroup();
	Vec3f background_color = sceneParser->getBackgroundColor();
	Vec3f ambient_light = sceneParser->getAmbientLight();

	resolution = width < height ? width : height;
	Image* image = new Image(resolution, resolution);
	image->SetAllPixels(background_color);
	Image* depth_image = new Image(resolution, resolution);
	depth_image->SetAllPixels(Vec3f(0.0, 0.0, 0.0));
	Image* normal_image = new Image(resolution, resolution);
	normal_image->SetAllPixels(Vec3f(0.0, 0.0, 0.0));

	float precalc = depth_max - depth_min;
	if (precalc == 0)
	{
		precalc = 1.0f;
	}

	RayTracer raytracer = RayTracer(sceneParser, bounce_limit, cut_off_ray_weight, cast_shadow);

	for (int j = 0; j < resolution; j++)
	{
		for (int i = 0; i < resolution; i++)
		{
			float x = (float)i * 2.0f / resolution - 1.0f;
			float y = (float)j * 2.0f / resolution - 1.0f;
			Material* material = sceneParser->getMaterial(0);
			Ray ray = camera->generateRay(Vec2f(x, y));
			Hit* hit = new Hit(INT_MAX, material, Vec3f(0.0f, 0.0f, 0.0f));
			float tmin = 0.0f;
			Vec3f pixel = raytracer.traceRay(ray, tmin, 1, 1.0f, hit->getMaterial()->getIndexOfRefraction(), *hit, shade_back, true);
			image->SetPixel(i, j, pixel);
			float t = hit->getT();
			if (t < depth_min)
			{
				t = depth_min;
			}
			if (t > depth_max)
			{
				t = depth_max;
			}
			float volumn = (depth_max - t) / precalc;
			depth_image->SetPixel(i, j, Vec3f(volumn, volumn, volumn));
			Vec3f normal = hit->getNormal();
			Vec3f normal_fab = Vec3f(fabs(normal.x()), fabs(normal.y()), fabs(normal.z()));
			normal_image->SetPixel(i, j, normal_fab);			
		}
	}
	
	if (output_file != NULL)
	{
		image->SaveTGA(output_file);
	}
	if (depth_file != NULL)
	{
		depth_image->SaveTGA(depth_file);
	}
	if (normal_file != NULL)
	{
		normal_image->SaveTGA(normal_file);
	}

	GLCanvas canvas;
	canvas.initialize(sceneParser, RenderScene, traceRayFunction);
	return 0;
}

