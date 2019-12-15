#pragma once
#include "curve.h"

class BSplineCurve : public Curve{
public:
	BSplineCurve(){}
	BSplineCurve(int num){
		vertices_num = num;
		vertices_array = new Vec3f[1000];
	}
	virtual ~BSplineCurve(){}

	virtual void Paint(ArgParser* args){
		Curve::Paint(args);
		int subdivision = args->curve_tessellation;
		for (int i = 0; i < vertices_num - 3; i++){
			glColor3f(0, 1, 0);
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j <= subdivision; j++) {
				GLfloat t = (GLfloat)j / ((GLfloat)subdivision);
				Vec3f vec = Get_BSpline_vertice(t, i);
				glVertex3f(vec.x(), vec.y(), vec.z());
			}
			glEnd();
		}
	}

	Vec3f Get_BSpline_vertice(GLfloat t, int index){
		float p1 = (-1.0 * (t * t * t) + 3.0 * (t * t) - 3.0 * t + 1.0) / 6.0;
		float p2 = (3.0 * (t * t * t) - 6.0 * (t * t) + 4.0) / 6.0;
		float p3 = (-3.0 * (t * t * t) + 3.0 * (t * t) + 3.0 * t + 1.0) / 6.0;
		float p4 = (t * t * t) / 6.0;
		
		float x = vertices_array[index].x() * p1 + vertices_array[index + 1].x() * p2 
			+ vertices_array[index + 2].x() * p3 + vertices_array[index + 3].x() * p4;
		float y = vertices_array[index].y() * p1 + vertices_array[index + 1].y() * p2 
			+vertices_array[index + 2].y() * p3 + vertices_array[index + 3].y() * p4;
		float z = vertices_array[index].z() * p1 + vertices_array[index + 1].z() * p2 
			+vertices_array[index + 2].z() * p3 + vertices_array[index + 3].z() * p4;
		
		return Vec3f(x, y, z);
	}

	virtual void OutputBSpline(FILE* file) {
		fprintf(file, "%s", "bspline num_vertices ");
		fprintf(file, "%d %s", vertices_num,"\n");
		for (int i = 0; i < vertices_num; i++) {
			fprintf(file, "%.1f %.1f %.1f % s", vertices_array[i].x(), vertices_array[i].y(), vertices_array[i].z(), "\n");
		}
	}

	virtual void OutputBezier(FILE* file){
		float* matrix_bspline;
		float* matrix_bezier;
		matrix_bezier = new float[16]{
			-1, 3, -3, 1,
			3, -6, 3, 0,
			-3, 3, 0, 0,
			1, 0, 0, 0 };
		matrix_bspline = new float[16]{
			-1, 3, -3, 1,
			3, -6, 0, 4,
			-3, 3, 3, 1,
			1, 0, 0, 0 };

		Matrix bspline = (1.0 / 6.0) * Matrix(matrix_bspline);
		Matrix bezier = Matrix(matrix_bezier);
		bezier.Inverse();
		Matrix B1andB2Inverse = bspline * bezier;
		int nums = 0;
		Vec3f* bezier_output[1000];
		for (int i = 0; i < vertices_num - 3; i++){
			float bspline_arr[12] = { vertices_array[i].x(), vertices_array[i + 1].x(), vertices_array[i + 2].x(), vertices_array[i + 3].x(),
										 vertices_array[i].y(), vertices_array[i + 1].y(), vertices_array[i + 2].y(), vertices_array[i + 3].y(),
										 vertices_array[i].z(), vertices_array[i + 1].z(), vertices_array[i + 2].z(), vertices_array[i + 3].z() };
			Matrix bspline_matrix = Matrix(bspline_arr);
			Matrix bezier_matrix = bspline_matrix * B1andB2Inverse;
			Vec3f res[4] = { Vec3f(bezier_matrix.Get(0, 0), bezier_matrix.Get(0, 1), bezier_matrix.Get(0, 2)),
									Vec3f(bezier_matrix.Get(1, 0), bezier_matrix.Get(1, 1), bezier_matrix.Get(1, 2)),
									Vec3f(bezier_matrix.Get(2, 0), bezier_matrix.Get(2, 1), bezier_matrix.Get(2, 2)),
									Vec3f(bezier_matrix.Get(3, 0), bezier_matrix.Get(3, 1), bezier_matrix.Get(3, 2)) };
			bezier_output[nums] = res;
			nums++;
		}
		for (int i = 0; i < nums; i++){
			fprintf(file, "%s", "bezier num_vertices 4\n ");
			fprintf(file, "%.1f %.1f %.1f % s", bezier_output[i][0].x(), bezier_output[i][0].y(), bezier_output[i][0].z(), "\n");
			fprintf(file, "%.1f %.1f %.1f % s", bezier_output[i][1].x(), bezier_output[i][1].y(), bezier_output[i][1].z(), "\n");
			fprintf(file, "%.1f %.1f %.1f % s", bezier_output[i][2].x(), bezier_output[i][2].y(), bezier_output[i][2].z(), "\n");
			fprintf(file, "%.1f %.1f %.1f % s", bezier_output[i][3].x(), bezier_output[i][3].y(), bezier_output[i][3].z(), "\n");
			fprintf(file, "%s", "\n");
		}
		delete[] matrix_bezier;
		delete[] matrix_bspline;
	}

	virtual TriangleMesh* OutputTriangles(ArgParser* args){
		int num_verts = (((float)vertices_num - 3.0f) * args->curve_tessellation + 1) * args->revolution_tessellation;
		int num_tris = ((float)vertices_num - 3.0f) * args->curve_tessellation * args->revolution_tessellation * 2;
		TriangleMesh* MyMesh = new TriangleMesh(num_verts, num_tris);
		for (int k = 0,p = 0; p < args->revolution_tessellation; p++){
			Vec3f tmp_vec = Vec3f(9999.0f, 9999.0f, 9999.0f);
			float delta = ((float)p / (float)args->revolution_tessellation) * 360;
			Matrix tmp;
			Matrix rotation_matrix = tmp.MakeAxisRotation(Vec3f(0.0f, 1.0f, 0.0f), delta / 180 * 3.14159);
			for (int i = 0; i < vertices_num - 3; i++){
				for (int j = 0; j <= args->curve_tessellation; j++){
					GLfloat t = (GLfloat)j / ((GLfloat)args->curve_tessellation);
					Vec3f vec = Get_BSpline_vertice(t, i);
					if (vec == tmp_vec)	continue;
					tmp_vec.Set(vec.x(), vec.y(), vec.z());
					Matrix v_matrix = Matrix();
					v_matrix.Set(0, 0, vec.x());
					v_matrix.Set(0, 1, vec.y());
					v_matrix.Set(0, 2, vec.z());
					Matrix result_matrix = rotation_matrix * v_matrix;
					Vec3f result = Vec3f(result_matrix.Get(0, 0), result_matrix.Get(0, 1), result_matrix.Get(0, 2));
					MyMesh->SetVertex(k, result);
					k++;
				}
			}
		}
		int circle = ((float)vertices_num - 3.0f) * args->curve_tessellation + 1;
		for (int n = 0, i = 0; i < num_verts; i++){
			if ((i + 1) % circle != 0){
				MyMesh->SetTriangle(n, i, (i + circle) % num_verts, (i + circle + 1) % num_verts);n++;
				MyMesh->SetTriangle(n, i, i + 1, (i + circle + 1) % num_verts);n++;
			}
		}
		return MyMesh;
	}
};