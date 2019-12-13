#ifndef SIMPLESYSTEM_H
#define SIMPLESYSTEM_H
#include <GL/glut.h>
#include <vecmath.h>
#include <vector>
#include <iostream>

#include "particleSystem.h"

using namespace std;

class SimpleSystem: public ParticleSystem
{
public:
	SimpleSystem();
	virtual vector<Vector3f> evalF(vector<Vector3f> state);
	virtual void draw();
};

#endif
