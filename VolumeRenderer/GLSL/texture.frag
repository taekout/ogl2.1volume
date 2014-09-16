#version 130

precision highp float;

in vec3 fragNormal;
in vec2 fragUV;
#ifdef Blinn
in vec3 V;
#endif

out vec4 outColor;

uniform sampler2D imageTexSampler;
uniform vec3 LightDir;

void main()
{
#ifdef Blinn
	vec3 R = normalize(2.0 * fragNormal * dot(fragNormal, LightDir) - LightDir);
	vec3 H = normalize(fragNormal + V);
	float specularity = max( dot(fragNormal, H), 0.0 );
#endif

	float cosTheta = clamp( dot(fragNormal, LightDir), 0.0, 1.0 );
	outColor.rgb = texture(imageTexSampler, fragUV).rgb;
	outColor = vec4( outColor.rgb * (cosTheta), 1.0 );

}

