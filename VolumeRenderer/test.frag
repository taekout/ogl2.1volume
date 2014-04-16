#version 130

precision highp float;

in vec3 fragNormal;
in vec2 fragUV;
in vec3 fragL;

out vec4 outColor;

uniform sampler2D imageTexSampler;

void main()
{
	float cosTheta = clamp( dot(fragL, fragNormal), 0.f, 1.f );
	outColor.rgb = texture(imageTexSampler, fragUV).rgb;
	outColor = vec4( outColor.rgb * cosTheta, 1.0 );
}

