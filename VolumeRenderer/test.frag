#version 130

precision highp float;

in vec3 fragNormal;
in vec2 fragUV;
in vec3 fragL;
in vec3 V;

out vec4 outColor;

uniform sampler2D imageTexSampler;

void main()
{
	vec3 R = normalize(2.f * fragNormal * dot(fragNormal, fragL) - fragL);
	float specularity = max( dot(R, V), 0.f );

	float cosTheta = clamp( dot(fragL, fragNormal), 0.f, 1.f );
	outColor.rgb = texture(imageTexSampler, fragUV).rgb;
	outColor = vec4( outColor.rgb * (cosTheta + specularity), 1.0 );
}

