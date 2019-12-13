#include "TimeStepper.hpp"
#include <iostream>
///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> newX;

    vector<Vector3f> currentX = particleSystem->getState();

    vector<Vector3f> f_X = particleSystem->evalF(currentX);


    for (int i=0; i<currentX.size(); i++){
	Vector3f eachX = currentX[i] + stepSize*f_X[i];
    	newX.push_back(eachX);
    }

    particleSystem->setState(newX);

}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> newX;
	
    vector<Vector3f> currentX = particleSystem->getState();

    vector<Vector3f> f_0 = particleSystem->evalF(currentX);
    vector<Vector3f> f_1_x;

    for (int x=0; x<f_0.size(); x++){
	Vector3f X_each = currentX[x] + (f_0[x]*stepSize);
    	f_1_x.push_back(X_each);
    }

    vector<Vector3f> f_1 = particleSystem->evalF(f_1_x);

    for (int i=0; i<currentX.size(); i++){
	Vector3f eachX = currentX[i] + ((f_0[i] + f_1[i])*(stepSize/2.0));
    	newX.push_back(eachX);
    }

    particleSystem->setState(newX);

}

