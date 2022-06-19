#pragma once
#include "Vertex.h"
class Camera
{
public:
	vec4 position;
	float* worldMatrix;
	float* viewMatrix;
	float* projectionMatrix;

	vec4 LocalToWorld();
	vec4 WorldToView();
	vec4 ViewToClip();


};

