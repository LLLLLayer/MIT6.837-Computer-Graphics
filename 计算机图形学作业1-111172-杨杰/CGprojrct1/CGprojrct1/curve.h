#pragma once
#include "spline.h"
class Curve : public Spline{
public:
	Curve(){}
	Curve(int num){
		vertices_num = vertices_num;
		vertices_array = new Vec3f[1000];
	}
	virtual ~Curve(){}

	virtual void Paint(ArgParser* args){
		Paint_Vertices();//绘制点
		Paint_Lines();//绘制线
	}
	void Paint_Vertices(){
		for (int i = 0; i < vertices_num; i++){
			float vx = 0, vy = 0, vz = 0;
			vertices_array[i].Get(vx, vy, vz);
			glPointSize(5.0);
			glBegin(GL_POINTS);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(vx, vy, vz);
			glEnd();
		}
	}
	void Paint_Lines(){
		for (int i = 0; i < vertices_num - 1; i++){
			float vx1 = 0, vy1 = 0, vz1 = 0;
			vertices_array[i].Get(vx1, vy1, vz1);
			float vx2 = 0, vy2 = 0, vz2 = 0;
			vertices_array[i + 1].Get(vx2, vy2, vz2);
			glBegin(GL_LINES);
			glLineWidth(5);
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(vx1, vy1, vz1);
			glVertex3f(vx2, vy2, vz2);
			glEnd();
		}
	}
};