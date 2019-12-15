#pragma once
#include "curve.h"

class BezierCurve : public Curve{
public:
	BezierCurve(){}
	BezierCurve(int num){
		vertices_num = num;
		vertices_array = new Vec3f[1000];
	}
	virtual ~BezierCurve(){}

	virtual void Paint(ArgParser* args){
		Curve::Paint(args);//控制点绘制
		int subdivision = args->curve_tessellation;
		for (int i = 0; i < vertices_num - 1;){//每由四个点通过函数绘制出曲线上的一个点
			glColor3f(0, 1, 0);
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j <= subdivision; j++) {//画每一段线段
				GLfloat t = (GLfloat)j / ((GLfloat)subdivision);
				Vec3f v = Get_Bezier_vertice(t, i);//获得线上点坐标
				glVertex3f(v.x(), v.y(), v.z());
			}
			glEnd();
			i += 3;
		}
	}
	Vec3f Get_Bezier_vertice(GLfloat t, int index) {
		float p1 = (1.0 - t) * (1.0 - t) * (1.0 - t);	//(1-t)^3
		float p2 = 3.0 * t * (1.0 - t) * (1.0 - t);		//3t(1-t)^2 
		float p3 = 3.0 * t * t * (1.0 - t);				//3t^2(1-t) 
		float p4 = t * t * t;							//t^3

		float x = vertices_array[index].x() * p1 + vertices_array[index + 1].x() * p2 
			+vertices_array[index + 2].x() * p3 + vertices_array[index + 3].x() * p4;
		float y = vertices_array[index].y() * p1 + vertices_array[index + 1].y() * p2 
			+vertices_array[index + 2].y() * p3 + vertices_array[index + 3].y() * p4;
		float z = vertices_array[index].z() * p1 + vertices_array[index + 1].z() * p2 
			+vertices_array[index + 2].z() * p3 + vertices_array[index + 3].z() * p4;
		
		return Vec3f(x, y, z);
	}

	virtual void OutputBezier(FILE* file){//Bezier的OutputBezier直接输出自己的点
		fprintf(file, "%s", "bezier num_vertices ");
		fprintf(file, "%d %s", vertices_num,"\n");
		for (int i = 0; i < vertices_num; i++){
			fprintf(file, "%.1f %.1f %.1f %s", vertices_array[i].x(), vertices_array[i].y(), vertices_array[i].z(), "\n");
		}
	}

	virtual void OutputBSpline(FILE* file){
		//获取曲线的矩阵
		float* matrix_bspline;
		float* matrix_bezier;
		matrix_bezier = new float[16]{ //Bbezier
			-1, 3, -3, 1,
			3, -6, 3, 0,
			-3, 3, 0, 0,
			1, 0, 0, 0 };
		matrix_bspline = new float[16]{ //Bbsspline
			-1, 3, -3, 1,
			3, -6, 0, 4,
			-3, 3, 3, 1,
			1, 0, 0, 0 };
		Matrix bspline = (1.0 / 6.0) * Matrix(matrix_bspline);
		Matrix bezier = Matrix(matrix_bezier);
		bspline.Inverse();
		Matrix B1andB2Inverse = bezier * bspline;//获得公式G1=G2*B1*B2^(-1)的B1*B2^(-1)

		int nums = 0;//统计点数用于输出
		Vec3f* bspline_output[1000];
		for (int i = 0; i < vertices_num - 1; i += 3){//带入公式G1=G2*B1*B2^(-1)，用G2乘后面的参数
			float bezier_arr[12] = { vertices_array[i].x(), vertices_array[i + 1].x(), vertices_array[i + 2].x(), vertices_array[i + 3].x(),
										 vertices_array[i].y(), vertices_array[i + 1].y(), vertices_array[i + 2].y(), vertices_array[i + 3].y(),
										 vertices_array[i].z(), vertices_array[i + 1].z(), vertices_array[i + 2].z(), vertices_array[i + 3].z() };
			Matrix bezier_matrix = Matrix(bezier_arr);
			Matrix bspline_matrix = bezier_matrix * B1andB2Inverse;//公式G1=G2*B1*B2^(-1)
			Vec3f res[4] = { Vec3f(bspline_matrix.Get(0, 0), bspline_matrix.Get(0, 1), bspline_matrix.Get(0, 2)),
									Vec3f(bspline_matrix.Get(1, 0), bspline_matrix.Get(1, 1), bspline_matrix.Get(1, 2)),
									Vec3f(bspline_matrix.Get(2, 0), bspline_matrix.Get(2, 1), bspline_matrix.Get(2, 2)),
									Vec3f(bspline_matrix.Get(3, 0), bspline_matrix.Get(3, 1), bspline_matrix.Get(3, 2)) };
			bspline_output[nums] = res;
			nums++;
		}
		for (int i = 0; i < nums; i++){
			fprintf(file, "%s", "bspline num_vertices 4\n ");
			fprintf(file, "%.1f %.1f %.1f %s", bspline_output[i][0].x(), bspline_output[i][0].y(), bspline_output[i][0].z(), "\n");
			fprintf(file, "%.1f %.1f %.1f %s", bspline_output[i][1].x(), bspline_output[i][1].y(), bspline_output[i][1].z(), "\n");
			fprintf(file, "%.1f %.1f %.1f %s", bspline_output[i][2].x(), bspline_output[i][2].y(), bspline_output[i][2].z(), "\n");
			fprintf(file, "%.1f %.1f %.1f %s", bspline_output[i][3].x(), bspline_output[i][3].y(), bspline_output[i][3].z(), "\n");
			fprintf(file, "%s", "\n");
		}
		delete[] matrix_bezier;
		delete[] matrix_bspline;
	}

	virtual TriangleMesh* OutputTriangles(ArgParser *args){
		int num_verts = (((float)vertices_num - 1.0f) * args->curve_tessellation + 1) * args->revolution_tessellation;
		int nem_tris = ((float)vertices_num - 1.0f) * args->curve_tessellation * args->revolution_tessellation * 2;
		TriangleMesh* MyMesh = new TriangleMesh(num_verts, nem_tris);
		for (int k=0,p = 0; p < args->revolution_tessellation; p++){
			Vec3f tmp_v = Vec3f(9999.0f, 9999.0f, 9999.0f);
			float delta = ((float)p / (float)args->revolution_tessellation) * 360;
			Matrix tmp;
			Matrix rotation_matrix = tmp.MakeAxisRotation(Vec3f(0.0f, 1.0f, 0.0f), delta / 180 * 3.14159);
			for (int i = 0; i < vertices_num - 1; i += 3){
				for (int j = 0; j <= args->curve_tessellation; j++){
					GLfloat t = (GLfloat)j / ((GLfloat)args->curve_tessellation);
					Vec3f v = Get_Bezier_vertice(t, i);
					if (v == tmp_v) continue;
					tmp_v.Set(v.x(), v.y(), v.z());			
					Matrix v_matrix = Matrix();
					v_matrix.Set(0, 0, v.x());
					v_matrix.Set(0, 1, v.y());
					v_matrix.Set(0, 2, v.z());
					Matrix result_matrix = rotation_matrix * v_matrix;
					Vec3f result = Vec3f(result_matrix.Get(0, 0), result_matrix.Get(0, 1), result_matrix.Get(0, 2));
					MyMesh->SetVertex(k, result);
					k++;
				}
			}
		}
		int circle = ((float)vertices_num - 1.0f) * args->curve_tessellation + 1;
		for (int n = 0, l = 0; n < num_verts; n++){
			if ((n + 1) % circle != 0){
				MyMesh->SetTriangle(l, n, (n + circle) % num_verts, (n + circle + 1) % num_verts);l++;
				MyMesh->SetTriangle(l, n, n + 1, (n + circle + 1) % num_verts);l++;
			}	
		}
		return MyMesh;
	}
};