
#pragma once

#include "vectors.h"
#include "matrix.h"
#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "rayTree.h"

class RayTracer
{
public:
	RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows)
	{
		this->scene_parser = s;
		this->max_bounces = max_bounces;
		this->shadows = shadows;
		this->cutoff_weight = cutoff_weight;
	}
	~RayTracer()
	{

	}
	
	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit, bool shade_back, bool first) const
	{
		Object3D* group = scene_parser->getGroup();
		int num_materials = scene_parser->getNumMaterials();
		int num_lights = scene_parser->getNumLights();
		Vec3f ambient_light = scene_parser->getAmbientLight();
		Vec3f pixel = scene_parser->getBackgroundColor();
		const float epsilon = 0.01f;


		if (group->intersect(ray, hit, tmin))
		{
			if (first)
			{
				RayTree::SetMainSegment(ray, 0.0f, hit.getT());
			}

			Vec3f color = hit.getMaterial()->getDiffuseColor();
			pixel = color * ambient_light;
			Vec3f normal = hit.getNormal();
			normal.Normalize();

			Vec3f intersect_point = hit.getIntersectionPoint();

			for (int p = 0; p < num_lights; p++)
			{
				Light* light = scene_parser->getLight(p);
				Vec3f light_direction;
				Vec3f light_color;
				float distance_to_light;
				light->getIllumination(intersect_point, light_direction, light_color, distance_to_light);
				Ray ray2 = Ray(intersect_point, light_direction);
				Hit hit2 = Hit(distance_to_light, NULL, Vec3f());

				if (shadows)
				{
					bool result = group->intersect(ray2, hit2, epsilon);
					RayTree::AddShadowSegment(ray2, 0.0f, hit2.getT());
					if (!result)
					{
						pixel = pixel + hit.getMaterial()->shade(ray, hit, light_direction, light_color, shade_back);
					}
				}
				else
				{
					pixel = pixel + hit.getMaterial()->shade(ray, hit, light_direction, light_color, shade_back);
				}
			}

			if (bounces <= max_bounces && weight >= cutoff_weight)
			{
				Vec3f _reflective_color = hit.getMaterial()->getReflectiveColor();
				if (hit.getMaterial()->getReflectiveColor().Length() > 0)
				{
					Vec3f reflective_direction = mirrorDirection(normal, ray.getDirection());
					Ray ray3 = Ray(intersect_point, reflective_direction);
					Hit hit3 = Hit(INT_MAX, NULL, Vec3f());
					Vec3f reflective_color = traceRay(ray3, epsilon, bounces + 1, weight * _reflective_color.Length(), indexOfRefraction, hit3, shade_back, false);
					RayTree::AddReflectedSegment(ray3, 0.0f, hit3.getT());
					pixel = pixel + reflective_color * _reflective_color;
				}

				float index_refraction;
				if (normal.Dot3(ray.getDirection() * -1.0f) > 0)
				{
					//outside
					index_refraction = hit.getMaterial()->getIndexOfRefraction();;
				}
				else
				{
					////inside
					index_refraction = 1.0f / indexOfRefraction;
					normal.Negate();
				}

				Vec3f transparent_color = hit.getMaterial()->getTransparentColor();
				Vec3f transmitted_direction;
				if (transparent_color.Length() > 0 && transmittedDirection(normal, ray.getDirection(), 1.0f, index_refraction, transmitted_direction))
				{
					Ray ray4 = Ray(intersect_point, transmitted_direction);
					Hit hit4 = Hit(INT_MAX, NULL, Vec3f());
					Vec3f transmitted_color = traceRay(ray4, epsilon, bounces + 1, weight * transparent_color.Length(), index_refraction, hit4, shade_back, false);
					RayTree::AddTransmittedSegment(ray4, 0.0f, hit4.getT());
					pixel = pixel + transmitted_color * transparent_color;
				}
			}
		}		
		return pixel;
	}
	
	Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const
	{
		//out = incoming 每 2 (incoming ﹞ normal) normal
		Vec3f out = (incoming - normal * 2.0f * incoming.Dot3(normal));
		out.Normalize();
		return out;
	}

	bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted) const
	{
		//[ 灰r (N ﹞ I) 每 ﹟(1 每 灰r^2 (1 每(N ﹞ I)^2)) ] N 每 灰r I
		//灰r = 灰i / 灰t = index_i / index_t
		//I = -incoming
		//N = normal
		bool result = false;
		Vec3f n = normal;
		float dot3 = n.Dot3(incoming * -1.0f);

		if (index_t > 0.0f) 
		{
			float index_r = index_i / index_t;
			float tmp = 1.0f - pow(index_r, 2.0f) * (1.0f - pow(dot3, 2.0f));
			if (tmp >= 0) 
			{
				transmitted = n * (float)(index_r * dot3 - sqrt(tmp)) + incoming * index_r;
				transmitted.Normalize();
				result = true;
			}
		}
		return result;
	}
protected:
	float cutoff_weight;
	int max_bounces;
	bool shadows;
	SceneParser* scene_parser;
};
