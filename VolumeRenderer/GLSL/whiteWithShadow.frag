#version 130

precision highp float;

in vec3 fragNormal;
in vec2 fragUV;

in vec4 ShadowCoord;

uniform vec3 LightDir;
uniform sampler2D shadowMap;

out vec4 outColor;

void main()
{
	float visibility = 1.0;
	if ( texture( shadowMap, ShadowCoord.xy ).z  <  ShadowCoord.z)
		visibility = 0.5;

	outColor = vec4( clamp( dot(fragNormal, LightDir) * visibility, 0., 1. ) );
	//outColor = vec4( texture(imageTexSampler, fragUV).r, 0., 0., 1. );
}

