#ifndef SIMPLEPENDULUM_H
#define SIMPLEPENDULUM_H

#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "pendulumSystem.h"

using namespace std;

class SimplePendulum: public PendulumSystem {
    public:
        SimplePendulum();
};

#endif
