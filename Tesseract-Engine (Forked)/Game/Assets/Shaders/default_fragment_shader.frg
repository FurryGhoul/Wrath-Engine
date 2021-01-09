#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;
uniform int hasTexture;

void main()
{
	color = vec4(ourColor, 1.0);
	if(hasTexture == 1)
	{
		color = texture(ourTexture, TexCoord);
	}
}

