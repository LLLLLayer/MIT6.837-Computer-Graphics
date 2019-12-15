#include "MatrixStack.h"

MatrixStack::MatrixStack(){
	// 用单位矩阵初始化矩阵堆栈。
	m_matrices.push_back(Matrix4f::identity());
}

void MatrixStack::clear(){
	// 恢复为仅包含单位矩阵。
	m_matrices.clear();
	m_matrices.push_back(Matrix4f::identity());
}

Matrix4f MatrixStack::top(){
	// 返回栈顶
	return m_matrices.back();
}

void MatrixStack::push( const Matrix4f& m ){
	//将m推入堆栈。
	//您的堆栈应具有OpenGL语义：
	//新的顶部应该是旧的顶部乘以m
	Matrix4f newTop = m_matrices.back() * m;
	m_matrices.push_back(newTop);
}

void MatrixStack::pop(){
	// 从堆栈中删除顶部元素
	m_matrices.pop_back();
}


