#pragma once
#include "Vertex.h"
class Vec
{
public:
	static vec3 normaliseVec3(vec3 vec);
	static vec3 crossProductVec3(vec3 A, vec3 B);
	static float dotProductVec3(vec3 A, vec3 B);

};

