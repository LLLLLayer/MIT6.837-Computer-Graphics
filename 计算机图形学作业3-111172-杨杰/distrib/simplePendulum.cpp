
#include "simplePendulum.h"
#include <iostream>

using namespace std;

SimplePendulum::SimplePendulum():PendulumSystem(2) {

    wireframe_ON = true;

    particles.push_back(Vector4f(-1.0f, 0.0f, 0.0f, 1.0f));
    particles.push_back(Vector4f(-2.0f, 0.0f, 0.5f, 0.0f));

    //float restLength = 1.0f;
    //float springConstant = 2.0f;

    springs.push_back(Vector4f(0.0f, 1.0f, 1.0f, 2.0f));

    initPendulum(particles, springs, faces);

}
    
