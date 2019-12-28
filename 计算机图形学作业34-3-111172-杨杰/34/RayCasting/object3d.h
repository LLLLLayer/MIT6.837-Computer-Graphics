
#pragma once
#include "ray.h"
#include "hit.h"

class Object3D{
public:
	Object3D(){}
	~Object3D(){}
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
protected:
	Material* material;
};

class Sphere : public Object3D{
public:
	Sphere(Vec3f center, float radius, Material* material){
		this->center = new Vec3f(center.x(), center.y(), center.z());
		this->radius = radius;
		this->material = material;
	}
	~Sphere(){
		delete center;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin){
		bool result = false;
		Vec3f direction = r.getDirection();
		Vec3f origin = r.getOrigin();
		
		float a = direction.Dot3(direction);
		float b = 2.0f * direction.Dot3(origin - (*center));
		float c = (origin - (*center)).Length() * (origin - (*center)).Length() - radius * radius;
		float delta = b * b - 4.0f * a * c;
		float t = INT_MAX * 1.0f;
		if (delta > 0){
			float sroot = sqrt(delta);
			float t1 = (-b - sroot) / (a * 2.0f);
			float t2 = (-b + sroot) / (a * 2.0f);
			if (t1 >= tmin){
				t = t1;
			}
			else if (t2 >= tmin){
				t = t2;
			}
			if (t < h.getT()){
				Vec3f normal = origin + direction * t - *center;
				normal.Normalize();
				h.set(t, material, normal, r);
			}
			result = true;
		}
		else{
			result = false;
		}
		return result;
	}
protected:
	Vec3f* center;
	float radius;
};

class Group : public Object3D{
public:
	Group(int num_objects){
		this->object3ds = new Object3D* [num_objects];
		this->num_objects = num_objects;
	}
	~Group(){
		delete[] object3ds;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin){
		bool intersected = false;
		for (int i = 0; i < num_objects; i++){
			if (object3ds[i]->intersect(r, h, tmin) == true){
				intersected = true;
			}
		}
		return intersected;
	}
	void addObject(int index, Object3D *obj){
		object3ds[index] = obj;
	}
protected:
	Object3D** object3ds;
	int num_objects;
};

class Plane : public Object3D{
public:
	Plane(Vec3f &normal, float d, Material *m){
		this->material = m;
		this->normal = new Vec3f(normal.x(), normal.y(), normal.z());
		this->normal->Normalize();
		this->d = d;
	}
	~Plane(){
		delete normal;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin){
		bool result = false;
		if (normal->Dot3(r.getDirection()) == 0){
			result = false;
		}
		float t = (d - normal->Dot3(r.getOrigin())) / normal->Dot3(r.getDirection());
		if (t > tmin){
			if (t < h.getT()){
				result = true;
				h.set(t, material, *normal, r);
			}
		}
		return result;
	}
protected:
	float d;
	Vec3f* normal;
};

class Triangle : public Object3D{
public:
	Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m){
		this->a = new Vec3f(a.x(), a.y(), a.z());
		this->b = new Vec3f(b.x(), b.y(), b.z());
		this->c = new Vec3f(c.x(), c.y(), c.z());
		this->material = m;
		this->normal = new Vec3f();
		Vec3f::Cross3(*normal, b - a, c - a);
		this->e1 = new Vec3f();
		this->e2 = new Vec3f();
		*(this->e1) = a - c;
		*(this->e2) = b - c;
	}
	~Triangle(){
		delete a;
		delete b;
		delete c;
		delete e1;
		delete e2;
		delete normal;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin){
		bool result = false;
		Vec3f l = r.getOrigin() - *c;
		Vec3f k = -1.0f * r.getDirection();
		float det = k.Dot3(*normal);
		if (det != 0){
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

			if (t > tmin && u >= 0 && v >= 0 && 1 - u - v >= 0){
				if (t < h.getT()){
					result = true;
					Vec3f n = Vec3f(normal->x(), normal->y(), normal->z());
					n.Normalize();
					h.set(t, material, n, r);
				}
			}
		}
		return result;
	}
protected:
	Vec3f* a;
	Vec3f* b;
	Vec3f* c;
	Vec3f* e1;	//a-c
	Vec3f* e2;	//b-c
	Vec3f* normal;
};

class Transform : public Object3D{
public:
	Transform(Matrix &m, Object3D *o){
		this->object3d = o;
		this->transform_matrix = new Matrix(m);
	}
	~Transform(){
		delete transform_matrix;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin){
		bool result = false;
		Matrix* tmp1_matrix = new Matrix(*transform_matrix);
		if (tmp1_matrix->Inverse()){
			Vec3f direction = r.getDirection();
			Vec3f origin = r.getOrigin();
			tmp1_matrix->Transform(origin);
			tmp1_matrix->TransformDirection(direction);
			Ray ray = Ray(origin, direction);
		    result = object3d->intersect(ray, h, tmin);
			if (result){
				Vec3f n = h.getNormal();
				Matrix* tmp2_matrix = new Matrix(*tmp1_matrix);
				tmp2_matrix->Transpose();
				tmp2_matrix->TransformDirection(n);
				delete tmp1_matrix;
				delete tmp2_matrix;
				n.Normalize();
				h.set(h.getT(), h.getMaterial(), n, ray);
			}
		}
		return result;
	}
protected:
	Object3D* object3d;
	Matrix* transform_matrix;
};
