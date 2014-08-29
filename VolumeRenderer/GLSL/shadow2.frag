#version 130

precision highp float;

out float outDepth;

void main()
{
	outDepth = gl_FragCoord.z;
}

