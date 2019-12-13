#ifndef SIMPLECHAIN_H
#define SIMPLECHAIN_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "pendulumSystem.h"

using namespace std;

class SimpleChain: public PendulumSystem {
    public:
        SimpleChain();
};

#endif
