#version 130

precision highp float;

in vec3 inPositions;
in vec3 inNormals;
//in vec3 inColors;
out vec3 fragColor;
out vec3 fragNormal;
out vec3 fragL;

uniform mat4 Proj;
uniform mat4 View;
uniform mat4 Model;
uniform vec3 lightPos;

void main()
{
	fragColor = vec3(1.0, 1.0, 1.0);
	fragNormal = inNormals;

	vec4 vertex = vec4(inPositions, 1.0);
	gl_Position = vec4(Proj * View * Model * vertex);

	fragL = normalize(lightPos - vec3(gl_Position));
}

