
#pragma once
#include "ray.h"
#include "hit.h"
#include "matrix.h"
#include "material.h"

// Included files for OpenGL Rendering
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>

class Object3D
{
public:
	Object3D()
	{

	}
	~Object3D()
	{

	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void paint() = 0;
protected:
	Material* material;
};

class Sphere : public Object3D
{
public:
	Sphere(Vec3f center, float radius, Material* material)
	{
		this->center = new Vec3f(center.x(), center.y(), center.z());
		this->radius = radius;
		this->material = material;
	}
	~Sphere()
	{
		delete center;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		// Ray: R(t) = origin + direction * t
		// Sphere: | X - center | = r
		bool result = false;
		Vec3f direction = r.getDirection();
		Vec3f origin = r.getOrigin();
		
		float a = direction.Dot3(direction);
		float b = 2.0f * direction.Dot3(origin - (*center));
		float c = (origin - (*center)).Length() * (origin - (*center)).Length() - radius * radius;
		float delta = b * b - 4.0f * a * c;
		float t = INT_MAX * 1.0f;
		if (delta >= 0)
		{
			float sroot = sqrt(delta);
			float t1 = (-b - sroot) / (a * 2.0f);
			float t2 = (-b + sroot) / (a * 2.0f);
			if (t1 >= tmin)
			{
				t = t1;
				result = true;
			}
			else if (t2 >= tmin)
			{
				t = t2;
				result = true;
			}
			if (t < h.getT())
			{
				Vec3f normal = origin + direction * t - *center;
				normal.Normalize();
				h.set(t, material, normal, r);
			}			
		}
		return result;
	}
	virtual void paint() 
	{
		material->glSetMaterial();
		glTranslatef(center->x(), center->y(), center->z());
		glutSolidSphere(radius, 100, 100);			
	}
protected:
	Vec3f* center;
	float radius;
};

class Group : public Object3D
{
public:
	Group(int num_objects)
	{
		this->object3ds = new Object3D* [num_objects];
		this->num_objects = num_objects;
	}
	~Group()
	{
		delete[] object3ds;
	}
	virtual void paint() 
	{
		for (int i = 0; i < num_objects; i++)
		{
			object3ds[i]->paint();
		}
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		bool intersected = false;
		for (int i = 0; i < num_objects; i++)
		{
			if (object3ds[i]->intersect(r, h, tmin) == true)
			{
				intersected = true;
			}
		}
		return intersected;
	}
	void addObject(int index, Object3D *obj)
	{
		object3ds[index] = obj;
	}
protected:
	Object3D** object3ds;
	int num_objects;
};

class Plane : public Object3D
{
public:
	Plane(Vec3f &normal, float d, Material *m)
	{
		this->material = m;
		this->normal = new Vec3f(normal.x(), normal.y(), normal.z());
		this->normal->Normalize();
		this->d = d;
	}
	~Plane()
	{
		delete normal;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		// Ray: R(t) = origin + direction * t
		// Plane : P dot n + d = 0
		bool result = false;
		if (normal->Dot3(r.getDirection()) == 0)
		{
			result = false;
		}
		float t = (d - normal->Dot3(r.getOrigin())) / normal->Dot3(r.getDirection());
		if (t >= tmin)
		{
			if (t < h.getT())
			{
				result = true;
				h.set(t, material, *normal, r);
			}
		}
		return result;
	}
	virtual void paint() 
	{
		Vec3f d1, d2, v(1, 0, 0);
		if (*normal == v)
		{
			v = Vec3f(0, 1, 0);
		}
		Vec3f::Cross3(d1, v, *normal);
		d1.Normalize();
		Vec3f::Cross3(d2, *normal, d1);
		d2.Normalize();
		d1 *= 10000;
		d2 *= 10000;
		Vec3f e1 = d1 + d2, e2 = d1*(-1) + d2, e3 = d1*(-1) - d2, e4 = d1 - d2;
		e1 += d * *normal;
		e2 += d * *normal;
		e3 += d * *normal;
		e4 += d * *normal;
		material->glSetMaterial();
		glBegin(GL_QUADS);
		glNormal3f(normal->x(), normal->y(), normal->z());
		glVertex3f(e1.x(), e1.y(), e1.z());
		glVertex3f(e2.x(), e2.y(), e2.z());
		glVertex3f(e3.x(), e3.y(), e3.z());
		glVertex3f(e4.x(), e4.y(), e4.z());
		glEnd();
	}
protected:
	float d;
	Vec3f* normal;
};

class Triangle : public Object3D
{
public:
	Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m)
	{
		this->a = new Vec3f(a.x(), a.y(), a.z());
		this->b = new Vec3f(b.x(), b.y(), b.z());
		this->c = new Vec3f(c.x(), c.y(), c.z());
		this->material = m;	
		this->e1 = new Vec3f();
		this->e2 = new Vec3f();
		*(this->e1) = b - a;
		*(this->e2) = c - a;
		this->normal = new Vec3f();
		Vec3f::Cross3(*normal, *e1, *e2);
	}
	~Triangle()
	{
		delete a;
		delete b;
		delete c;
		delete e1;
		delete e2;
		delete normal;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		//e1 = b - a
		//e2 = c - a
		//l = o - a
		//k = -d
		//e1 * alpha+e2 * beta - t * direction = origin - a
		//t = dett / det, u = detu / det, v = detv / det 
		//det = k * (e1 x e2) = - d * n 
		//dett = l * (e1 x e2) = (o - c) * n
		//detu = k * (L x e2) = -d *[(o - a) x e2]
		//detv = k * (e1 x L) = -d *[e1 x(o - a)]
		bool result = false;
		Vec3f l = r.getOrigin() - *a;
		Vec3f k = -1.0f * r.getDirection();
		float det = k.Dot3(*normal);
		if (det != 0)
		{
			float dett = l.Dot3(*normal);
			Vec3f tmp1 = Vec3f();
			Vec3f tmp2 = Vec3f();
			Vec3f::Cross3(tmp1, l, *e2);
			Vec3f::Cross3(tmp2, *e1, l);
			float detu = k.Dot3(tmp1);
			float detv = k.Dot3(tmp2);

			float t = dett / det;
			float u = detu / det;
			float v = detv / det;

			if (t > tmin && u >= 0 && v >= 0 && 1 - u - v >= 0)
			{
				if (t < h.getT())
				{
					result = true;
					Vec3f n = Vec3f(normal->x(), normal->y(), normal->z());
					n.Normalize();
					h.set(t, material, n, r);
				}
			}
		}
		return result;
	}

	virtual void paint() 
	{
		material->glSetMaterial();
		glBegin(GL_TRIANGLES);
		glNormal3f(normal->x(), normal->y(), normal->z());
		glVertex3f(a->x(), a->y(), a->z());
		glVertex3f(b->x(), b->y(), b->z());
		glVertex3f(c->x(), c->y(), c->z());
		glEnd();
	}

protected:
	Vec3f* a;
	Vec3f* b;
	Vec3f* c;
	Vec3f* e1;	//a-c
	Vec3f* e2;	//b-c
	Vec3f* normal;
};

class Transform : public Object3D
{
public:
	Transform(Matrix &m, Object3D *o)
	{
		this->object3d = o;
		this->transform_matrix = new Matrix(m);
	}
	~Transform()
	{
		delete transform_matrix;
	}
	virtual void paint() 
	{
		glPushMatrix();
		Matrix tmp = Matrix(*transform_matrix);
		GLfloat *glMatrix = tmp.glGet();
		glMultMatrixf(glMatrix);
		delete[] glMatrix;
		object3d->paint();
		glPopMatrix();
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		bool result = false;
		Matrix* tmp1_matrix = new Matrix(*transform_matrix);
		if (tmp1_matrix->Inverse())
		{
			Vec3f direction = r.getDirection();
			Vec3f origin = r.getOrigin();
			tmp1_matrix->Transform(origin);
			tmp1_matrix->TransformDirection(direction);
			Ray ray = Ray(origin, direction);
		    result = object3d->intersect(ray, h, tmin);
			if (result)
			{
				Vec3f n = h.getNormal();
				Matrix* tmp2_matrix = new Matrix(*tmp1_matrix);
				tmp2_matrix->Transpose();
				tmp2_matrix->TransformDirection(n);
				delete tmp1_matrix;
				delete tmp2_matrix;
				n.Normalize();
				h.set(h.getT(), h.getMaterial(), n, r);
			}
		}
		return result;
	}
protected:
	Object3D* object3d;
	Matrix* transform_matrix;
};
