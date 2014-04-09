#version 130

precision highp float;

in vec3 fragColor;
in vec3 fragNormal;
in vec3 fragL;
out vec4 outColor;

void main()
{
	//outColor = vec4(0.0, 0.0, 0.0, 1.0);
	//outColor = vec4(fragColor, 1.0);
	float cosTheta = clamp( dot(fragL, fragNormal), 0.f, 1.f );
	outColor = vec4( fragColor * cosTheta, 1.0 );
}

