#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <GL/glut.h>
#include <vecmath.h>
#include <vector>

#include "pendulumSystem.h"

class ClothSystem: public PendulumSystem
{

public:
	ClothSystem(int C_SIZE);

};

#endif
