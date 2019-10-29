#version 330 core
out vec4 fragColor;

in Data {
	vec3 position;
	vec3 normal;
} data;

uniform vec3 lightPosition;

void main()
{
	
    fragColor = vec4(45,45,45, 1.0);

	fragColor = fragColor * dot(data.normal, lightPosition);
}