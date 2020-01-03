
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "hit.h"
#include "ray.h"
// ====================================================================
// ====================================================================

// You will extend this class in later assignments
// Included files for OpenGL Rendering
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>

class Material {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Material(const Vec3f &d_color) { diffuseColor = d_color; }
	virtual ~Material() {}

	// ACCESSORS
	Vec3f getDiffuseColor() const { return diffuseColor; }
	Vec3f getSpecularColor() const { return specularColor; };
	Vec3f getReflectiveColor() const { return reflectiveColor; };
	Vec3f getTransparentColor() const { return transparentColor; };
	float getIndexOfRefraction() const{ return indexOfRefraction; };
	float getExponent() const { return exponent; };
	virtual void glSetMaterial() const = 0;

	virtual Vec3f shade(Ray ray, Hit hit, Vec3f directionToLight, Vec3f lightColor, bool shade_back)
	{
		return Vec3f(0.0f, 0.0f, 0.0f);
	}
protected:

	// REPRESENTATION
	Vec3f diffuseColor;
	Vec3f specularColor;
	Vec3f reflectiveColor;
	Vec3f transparentColor;

	float indexOfRefraction;
	float exponent;
};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material
{
public:
	~PhongMaterial()
	{

	}

	PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent,
		const Vec3f &reflectiveColor, const Vec3f &transparentColor, float indexOfRefraction) : Material(diffuseColor)
	{
		this->specularColor = specularColor;
		this->reflectiveColor = reflectiveColor;
		this->transparentColor = transparentColor;
		this->indexOfRefraction = indexOfRefraction;
		this->exponent = exponent;
	}

	virtual Vec3f shade(Ray ray, Hit hit, Vec3f directionToLight, Vec3f lightColor, bool shade_back)
	{
		
		Vec3f direction = ray.getDirection();
		Vec3f normal = hit.getNormal();
		if (shade_back && direction.Dot3(normal) > 0)
		{
			normal.Negate();
		}

		float diffuse = directionToLight.Dot3(normal);
		//R = ¨CL + 2 (L¡¤N)N
		Vec3f reflection_out = normal * 2.0f * normal.Dot3(directionToLight) - directionToLight;
		float specular = -1.0f * direction.Dot3(reflection_out);
		if (diffuse < 0.0f)
		{
			diffuse = 0.0f;
		}
		if (specular < 0.0f)
		{
			specular = 0.0f;
		}
		
		Vec3f color = this->getDiffuseColor();
		color = color * diffuse + specularColor * pow(specular, exponent);
		color = color * lightColor;
		return color;
	}

	virtual void PhongMaterial::glSetMaterial() const
	{
		GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
		GLfloat specular[4] = {
			getSpecularColor().r(),
			getSpecularColor().g(),
			getSpecularColor().b(),
			1.0 };
		GLfloat diffuse[4] = {
			getDiffuseColor().r(),
			getDiffuseColor().g(),
			getDiffuseColor().b(),
			1.0 };

		// NOTE: GL uses the Blinn Torrance version of Phong...      
		float glexponent = exponent;
		if (glexponent < 0) glexponent = 0;
		if (glexponent > 128) glexponent = 128;

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
	}
};

#endif
