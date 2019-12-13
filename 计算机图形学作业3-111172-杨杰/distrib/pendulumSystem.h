#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H
#include <vecmath.h>
#include <vector>
#include <GL/glut.h>

#include "particleSystem.h"

extern int degree;	//Global variable for sinusoidalMovement
extern float xTrans;	//Global variable to make bal interactive
extern float yTrans;

using namespace std;

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);

	virtual vector<Vector3f> evalF(vector<Vector3f> state);

	//Construct Specific Pendulum
	void initPendulum(vector<Vector4f> p, vector<Vector4f> s, vector<Vector3f> f);
	
	virtual void draw();

	void wireframeToggle() {wireframe_ON = !wireframe_ON; faces_ON = !faces_ON;};
	void windToggle() {wind_ON = !wind_ON;};
	void sinusoidMovementToggle() {sinusoidMove_ON = !sinusoidMove_ON;};

	double convertRad(int d) { return d*(M_PI/180.0); };


protected:

	//Force Constants

	float mg;
	float vDrag;

	bool wireframe_ON;
	bool wind_ON;
	bool sinusoidMove_ON;
	bool faces_ON;
	bool particles_ON;
	bool structSprings_ON;

	bool isCloth;

	//Spring ( from p0, to p1, restLength, springConstant) 
	vector<Vector4f> springs;

	//Particle ( Vector3f pos, bool fixed)
	vector<Vector4f> particles;

	vector<Vector3f> faces;

};

#endif
