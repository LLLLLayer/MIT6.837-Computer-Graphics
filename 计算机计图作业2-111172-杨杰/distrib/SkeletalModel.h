#ifndef SKELETALMODEL_H
#define SKELETALMODEL_H

#ifdef WIN32
#include <windows.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

#include <cstdlib>
#ifdef WIN32
#include "GL/freeglut.h"
#include "FL/gl.h"
#else
#include <GL/glut.h>
#include <FL/gl.h>
#endif
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <vecmath.h>

#include "tuple.h"
#include "Joint.h"
#include "Mesh.h"
#include "MatrixStack.h"

class SkeletalModel
{
public:
	// Already-implemented utility functions that call the code you will write.
	void load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile);
	void draw(Matrix4f cameraMatrix, bool drawSkeleton);

	// Part 1: Understanding Hierarchical Modeling

	// 1.1. Implement method to load a skeleton.
	// This method should compute m_rootJoint and populate m_joints.
	//第1部分：了解层次建模

	// 1.1。 实施方法以加载骨架。
	//此方法应计算m_rootJoint并填充m_joints。
	void loadSkeleton( const char* filename );

	// 1.1. Implement this method with a recursive helper to draw a sphere at each joint.
	// 1.1。 使用递归助手实现此方法，以在每个关节处绘制一个球体。
	void drawJoints( );

	// 1.2. Implement this method a recursive helper to draw a box between each pair of joints
	// 1.2。 将此方法实现为递归帮助器，以在每对关节之间绘制一个框
	void drawSkeleton( );

	// 1.3. Implement this method to handle changes to your skeleton given
	// changes in the slider values
	// 1.3。 实现此方法以处理给定的骨架更改
	//滑块值的更改
	void setJointTransform( int jointIndex, float rX, float rY, float rZ );

	// Part 2: Skeletal Subspace Deformation

	// 2.3. Implement SSD

	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//第2部分：骨骼子空间变形

	// 2.3。 实施SSD

	// 2.3.1。 实现此方法以从中计算每个关节的变换
	// BIND POSE中的世界空间到联合空间。
	void computeBindWorldToJointTransforms();

	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	// 2.3.2。 实现此方法以从中计算每个关节的变换
	//在当前姿势中将空间联合到世界空间。
	void updateCurrentJointToWorldTransforms();

	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
	// 2.3.2。 这是SSD的核心。
	//实现此方法以更新网格的顶点
	//给出骨架的当前状态。
	//您将需要两个绑定姿势世界->联合变换。
	//和当前的联合->世界将发生变化。
	void updateMesh();

private:

	// pointer to the root joint
	//指向根关节的指针
	Joint* m_rootJoint;
	// the list of joints.
	//关节列表。
	std::vector< Joint* > m_joints;

	Mesh m_mesh;
	MatrixStack m_matrixStack;

	void dfsJoint(Joint* joint, MatrixStack ms);
	void dfsBone(Joint* joint, MatrixStack ms, Joint* top);
};

#endif
