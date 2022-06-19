#pragma once
#include "Vertex.h"
class Vec
{
public:
	static vec3 normaliseVec3(vec3 vec);
	static vec3 crossProductVec3(vec3 A, vec3 B);
	static float dotProductVec3(vec3 A, vec3 B);
	static vec3 vec4ToVec3(vec4 A);
	static vec3 add(vec3 A, vec3 B);
	static vec3 multiplyByFloat(vec3 A, float B);
	static vec3 sub(vec3 A, vec3 B);
};

