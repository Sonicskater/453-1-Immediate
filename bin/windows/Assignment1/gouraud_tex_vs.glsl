#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;


out vec3 outColor; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float tex;

uniform sampler2D texture;

uniform vec3 lightPos;
uniform vec3 lookPos;

uniform float ambient;

void main()
{
	
    gl_Position = projection * view * model * vec4(aPosition, 1.0);

	vec3 normal = mat3(transpose(inverse(model))) * aNormal; 
	vec3 position = vec3(model * vec4(aPosition, 1.0));

	vec3 lightColor = vec3(1,1,1);

	// ambient
    float ambientStrength = 0.1;
    vec3 ambientv = ambient * lightColor;

	// diffuse
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(lookPos - position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;   

	//combine them
    outColor = ambientv + diffuse + specular;
}