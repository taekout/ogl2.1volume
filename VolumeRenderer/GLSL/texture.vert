#version 130

precision highp float;

in vec3 inPositions;
in vec3 inNormals;
in vec2 inUV;

//in vec3 inColors;
out vec3 fragNormal;
out vec2 fragUV;
out vec3 fragL;
#ifdef Blinn
out vec3 V;
#endif

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;
uniform mat4 NormalMat;
uniform vec3 LightPos;
uniform vec3 EyePos;

void main()
{
	fragNormal = (NormalMat * vec4(inNormals, 0.0)).xyz;

	vec4 vertexInEye = vec4(View * Model * vec4(inPositions, 1.0f));
#ifdef Blinn
	V = normalize(EyePos - vec3(gl_Position));
#endif

	fragL = normalize(LightPos - vertexInEye.xyz);
	fragUV = inUV;

	gl_Position = Proj * vertexInEye;
}

