#version 130

precision highp float;

in vec3 fragNormal;
in vec3 fragL;

out vec4 outColor;

void main()
{
	//float cosTheta = clamp( dot(fragNormal, fragL), 0.f, 1.f );
	outColor = vec4( clamp( dot(fragNormal, fragL), 0.f, 1.f ) );

	float d = gl_FragCoord.z;
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
	}
}

