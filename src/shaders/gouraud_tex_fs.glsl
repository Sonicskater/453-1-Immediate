#version 330 core
out vec4 fragColor;

in vec3 outColor; 
in vec2 texCoord;

uniform vec3 lightPosition;

uniform float tex;

uniform sampler2D textur;

void main()
{
	
    vec3 color = texture(textur, texCoord).rgb;


	fragColor = vec4(color * outColor, 1.0);
}