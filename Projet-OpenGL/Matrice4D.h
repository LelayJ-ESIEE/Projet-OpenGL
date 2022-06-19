#pragma once
#include "Vertex.h"
class Matrice4D{
	public:
		static float* identity();
		static float* projection(float right, float left, float top, float bottom, float far, float near);
		static float* perspective(float f, float aspect, float far, float near);
		static float* translation(float x, float y, float z);
		static float* rotation(float x, float y, float z);
		static float* scale(float x, float y, float z);
		static float* multiplie(float* a, float* b);
		static float* add(float* a, float* b);
		static float* LookAt(vec3 position, vec3 target, vec3 up);
};

