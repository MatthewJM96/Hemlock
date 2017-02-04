#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

uniform mat4 MVP;

out vec4 vertexColour;
out vec2 textureCoord;

void main(){

	//gl_Position = MVP * vec4(position, 1.0f);
	gl_Position = vec4(position, 1.0f);
	
	vertexColour = vec4(colour);
	textureCoord = texCoord;
}

