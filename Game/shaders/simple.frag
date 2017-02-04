#version 330 core

in vec4 vertexColour;
in vec2 textureCoord;

// Ouput data
out vec4 color;

uniform sampler2D tex;

void main()
{

	color = texture(tex, textureCoord) * vertexColour;
}