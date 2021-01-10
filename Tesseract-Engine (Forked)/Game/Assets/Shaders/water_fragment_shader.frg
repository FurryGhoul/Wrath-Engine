#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in float vertexHeight;

out vec4 color;

uniform sampler2D ourTexture;
uniform int hasTexture;
uniform float height;

void main()
{
	vec3 white = vec3(1.0, 0.9, 0.0);
	vec3 blue = vec3(0.0);
	vec3 transitionColor = vec3(vertexHeight/height);
	vec3 finalColor = smoothstep(blue, white, transitionColor);
	color = vec4(finalColor, 1.0);
}






