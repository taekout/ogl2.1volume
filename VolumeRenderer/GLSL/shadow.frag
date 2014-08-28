#version 130

precision highp float;

out vec4 outColor;

void main()
{
	outColor = gl_FragCoord.z;
}

