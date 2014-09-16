#version 130

precision highp float;

in vec3 fragNormal;

uniform vec3 LightDir;

out vec4 outColor;

void main()
{
	//float cosTheta = clamp( dot(fragNormal, LightDir), 0.f, 1.f );
	outColor = vec4( clamp( dot(fragNormal, LightDir), 0., 1. ) );

}

