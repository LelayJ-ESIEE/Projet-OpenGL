#version 120


// SORTIE DU VERTEX SHADER et INPUT du FRAGMENT SHADER
varying vec2 v_texcoords;
varying vec4 v_color;

uniform sampler2D u_sampler;

void main(void) {

    vec4 texColor = texture2D(u_sampler, v_texcoords);

    gl_FragColor = texColor;
}
