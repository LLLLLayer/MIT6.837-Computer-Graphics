#include "simpleSystem.h"
#include <iostream>

using namespace std;

SimpleSystem::SimpleSystem()
{
    m_vVecState.push_back(Vector3f(1.0f, 0.0, 0.0));
    
}

// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

	for (int s=0; s<state.size(); s++){
	    Vector3f derivF = Vector3f(-1*state[s][1], state[s][0], 0.0);
	    f.push_back(derivF);
	}

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
	Vector3f pos = getState()[0];
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2] );
	glutSolidSphere(0.075f,10.0f,10.0f);
	glPopMatrix();

}
