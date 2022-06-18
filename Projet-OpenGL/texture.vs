#version 120

// attribute = input du VERTEX shader

attribute vec3 a_position;
attribute vec2 a_texcoords;
attribute vec3 a_color;
attribute mat2 a_mat2;

// varying = output du VERTEX shader et INPUT du FRAGMENT shader
varying vec2 v_texcoords;
varying vec4 v_color;

uniform float u_time;

void main(void) 
{
	// stpq alias de xyzw alias de rgba
	v_texcoords = vec2(a_texcoords.s, 1.0 - a_texcoords.t);
	//v_mat = mat2(a_mat2[0],a_mat2[1]);
	gl_Position = vec4(a_position, 1.0); 
	// X=a_position.x;Y=a_position.y;Z=0.0;W=1.0

	// xyzw ou rgba
	v_color = vec4(a_color, 1.0);
}