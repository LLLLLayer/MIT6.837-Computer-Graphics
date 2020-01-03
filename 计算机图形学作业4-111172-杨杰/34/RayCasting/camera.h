
#pragma once
#include "ray.h"
#include "vectors.h"
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>

class Camera
{
public:
	Camera()
	{

	}

	~Camera()
	{

	}

	virtual Ray generateRay(Vec2f point) = 0;

	virtual float getTMin() const = 0;

	//place an orthographic camera within an OpenGL scene
	virtual void glPlaceCamera() = 0;

	//move camera along the direction vector
	virtual void dollyCamera(float dist) = 0;

	//rotate around the up and horizontal vectors
	virtual void rotateCamera(float x_theta, float y_theta) = 0;

	virtual void glInit(int w, int h) = 0;

	//translate camera perpendicular to the direction vector
	virtual void truckCamera(float dx, float dy) = 0;
};

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size)
	{
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

	~OrthographicCamera()
	{
		delete center;
		delete direction;
		delete up;
		delete horizontal;
	}

	virtual Ray generateRay(Vec2f point)
	{
		Vec3f origin = *center + *horizontal * point.x() * size * 0.5f + *up * point.y() * size * 0.5f;
		return Ray(origin, *direction);
	}

	virtual float getTMin() const
	{
		return -1.0f * INT_MAX;
	}
	
	virtual void glPlaceCamera()
	{
		gluLookAt(center->x(), center->y(), center->z(), center->x() + direction->x(), center->y() + direction->y(), center->z() + direction->z(), up->x(), up->y(), up->z());
	}
	
	virtual void dollyCamera(float dist)
	{
		*center = *center + *direction * dist;
	}
	
	virtual void rotateCamera(float x_theta, float y_theta)
	{
		float tiltAngle = acos(up->Dot3(*direction));
		if (tiltAngle - y_theta > 3.13)
			y_theta = tiltAngle - 3.13;
		else if (tiltAngle - y_theta < 0.01)
			y_theta = tiltAngle - 0.01;

		Matrix rotMat = Matrix::MakeAxisRotation(*up, x_theta);
		rotMat *= Matrix::MakeAxisRotation(*horizontal, y_theta);

		rotMat.Transform(*center);
		rotMat.TransformDirection(*direction);
		direction->Normalize();
	}

	virtual void glInit(int w, int h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (w > h)
		{
			glOrtho(-size / 2.0f, size / 2.0f, -size*(float)h / (float)w / 2.0, size*(float)h / (float)w / 2.0, 0.5, 40.0);
		}		
		else
		{
			glOrtho(-size*(float)w / (float)h / 2.0, size*(float)w / (float)h / 2.0, -size / 2.0, size / 2.0, 0.5, 40.0);
		}			
	}

	virtual void truckCamera(float dx, float dy)
	{
		Vec3f screenUp;
		Vec3f::Cross3(screenUp, *horizontal, *direction);
		*center += *horizontal * dx + screenUp * dy;
	}
protected:
	Vec3f* center;
	Vec3f* direction;
	Vec3f* horizontal;
	Vec3f* up;
	float size;
};

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle)
	{
		this->center = new Vec3f(center.x(), center.y(), center.z());
		this->direction = new Vec3f(direction.x(), direction.y(), direction.z());
		this->up = new Vec3f(up.x(), up.y(), up.z());
		this->horizontal = new Vec3f();
		Vec3f::Cross3(*this->horizontal, *this->direction, *this->up);
		Vec3f::Cross3(*this->up, *this->horizontal, *this->direction);
		this->direction->Normalize();
		this->up->Normalize();
		this->horizontal->Normalize();
		this->angle = angle;
		this->distance = 1.0f / tanf(angle / 2.0);
	}

	~PerspectiveCamera()
	{
		delete center;
		delete direction;
		delete up;
		delete horizontal;
	}

	virtual Ray generateRay(Vec2f point)
	{	
		Vec3f _direction = *horizontal * point.x() + *up * point.y() + (*direction) * distance;
		_direction.Normalize();
		return Ray(*center, _direction);
	}

	virtual float getTMin() const
	{
		return -1.0f * INT_MAX;
	}

	virtual void glPlaceCamera()
	{
		gluLookAt(center->x(), center->y(), center->z(), center->x() + direction->x(), center->y() + direction->y(), center->z() + direction->z(), up->x(), up->y(), up->z());
	}

	virtual void dollyCamera(float dist)
	{
		*center = *center + *direction * dist;
	}

	virtual void rotateCamera(float x_theta, float y_theta)
	{
		float tiltAngle = acos(up->Dot3(*direction));
		if (tiltAngle - y_theta > 3.13)
			y_theta = tiltAngle - 3.13;
		else if (tiltAngle - y_theta < 0.01)
			y_theta = tiltAngle - 0.01;

		Matrix rotMat = Matrix::MakeAxisRotation(*up, x_theta);
		rotMat *= Matrix::MakeAxisRotation(*horizontal, y_theta);

		rotMat.Transform(*center);
		rotMat.TransformDirection(*direction);
		direction->Normalize();
	}

	virtual void glInit(int w, int h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(angle *180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
	}

	virtual void truckCamera(float dx, float dy)
	{
		Vec3f screenUp;
		Vec3f::Cross3(screenUp, *horizontal, *direction);
		*center += *horizontal * dx + screenUp * dy;
	}
protected:
	Vec3f* center;
	Vec3f* direction;
	Vec3f* horizontal;
	Vec3f* up;
	float angle;
	float distance;
};
