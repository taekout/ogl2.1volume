#version 130

precision highp float;

in vec3 fragNormal;
in vec3 fragL;

out vec4 outColor;

void main()
{
	//float cosTheta = clamp( dot(fragNormal, fragL), 0.f, 1.f );
	outColor = vec4( clamp( dot(fragNormal, fragL), 0.f, 1.f ) );
}

