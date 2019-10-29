#version 330 core
out vec4 FragColor;

in Data {
	vec3 position;
	vec3 normal;
} data;

uniform vec3 lightPos;
uniform vec3 lookPos;

uniform float ambient;

uniform float tex;

uniform sampler2D texture;

void main()
{

	// sourced from stackexchange because geo doesnt work.
	vec3 xTangent = dFdx( data.position );
    vec3 yTangent = dFdy( data.position );
	vec3 normal = normalize( cross( xTangent, yTangent ) );

    vec3 color = vec3(1.0, 0.5, 0.31);

	vec3 lightColor = vec3(1,1,1);

	// ambient
    float ambientStrength = 0.1;
    vec3 ambientv = ambient * lightColor;

	//diffuse
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - data.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(lookPos - data.position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;   

	//combine them
    vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.0);
}