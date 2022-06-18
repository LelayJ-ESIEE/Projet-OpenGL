#version 120

// attribute = input du VERTEX shader

attribute vec3 a_position;
attribute vec2 a_texcoords;
attribute vec3 a_color;

// varying = output du VERTEX shader et INPUT du FRAGMENT shader
varying vec2 v_texcoords;
varying vec4 v_color;

uniform mat4 u_projection;
uniform mat4 u_translation;

void main(void) 
{
	// stpq alias de xyzw alias de rgba
	v_texcoords = vec2(a_texcoords.s, 1.0 - a_texcoords.t);

	gl_Position = u_projection * u_translation * vec4(a_position, 1.0); //en changeant la pos, on éloigne le PDV/"caméra"

}