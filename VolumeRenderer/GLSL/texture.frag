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
	vec3 R = normalize(2.f * fragNormal * dot(fragNormal, LightDir) - LightDir);
	vec3 H = normalize(fragNormal + V);
	float specularity = max( dot(fragNormal, H), 0.f );
#endif

	float cosTheta = clamp( dot(fragNormal, LightDir), 0.f, 1.f );
	outColor.rgb = texture(imageTexSampler, fragUV).rgb;
	outColor = vec4( outColor.rgb * (cosTheta), 1.0 );

	/*float d = gl_FragCoord.z;
	if(d > 0.9) {
		outColor = vec4( d, d, d, 1.f );
	}
	else if(d > 0.7) {
		outColor = vec4( d, 0.f, 0.f, 1.f );
	}
	else if(d > 0.5) {
		outColor = vec4( 0.f, d, 0.f, 1.f );
	}
	else if(d > 0.3) {
		outColor = vec4( 0.f, 0.f, d, 1.f );
	}
	else {
		outColor = vec4( 0.f, 0.f, 0.f, 1.f );
	}*/
}

