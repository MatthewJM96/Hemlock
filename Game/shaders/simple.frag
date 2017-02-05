#version 330 core

in vec4 vertexColour;
in vec2 textureCoord;

// Ouput data
out vec4 color;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{

	color = mix(texture(tex1, textureCoord), texture(tex2, textureCoord), 0.2);// * vertexColour;
	//color = texture(tex1, textureCoord);
}