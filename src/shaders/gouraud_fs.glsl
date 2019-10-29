#version 330 core
out vec4 fragColor;

in vec3 outColor; 

uniform vec3 lightPosition;

uniform float tex;

void main()
{
	
    vec3 color = vec3(1.0, 0.5, 0.31);

	fragColor = vec4(color * outColor, 1.0);
}