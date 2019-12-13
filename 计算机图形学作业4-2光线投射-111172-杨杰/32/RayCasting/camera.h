#pragma once
#include "ray.h"
#include "vectors.h"

class Camera{
public:
	Camera(){}
	~Camera(){}
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;
};

class OrthographicCamera : public Camera{
public:
	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size){
		this->center = new Vec3f(center.x(), center.y(), center.z());
		this->direction = new Vec3f(direction.x(), direction.y(), direction.z());
		this->up = new Vec3f(up.x(), up.y(), up.z());
		this->horizontal = new Vec3f();
		Vec3f::Cross3(*this->horizontal, *this->direction, *this->up);
		Vec3f::Cross3(*this->up, *this->horizontal, *this->direction);
		this->direction->Normalize();
		this->up->Normalize();
		this->horizontal->Normalize();
		this->size = size;
	}
	~OrthographicCamera(){
		delete center;
		delete direction;
		delete up;
		delete horizontal;
	}
	virtual Ray generateRay(Vec2f point){
		Vec3f origin = *center + *horizontal * point.x() * size * 0.5 + *up * point.y() * size * 0.5;
		return Ray(origin, *direction);
	}
	virtual float getTMin() const{
		return -1.0f * INT_MAX;
	}
protected:
	Vec3f* center;
	Vec3f* direction;
	Vec3f* horizontal;
	Vec3f* up;
	float size;
};