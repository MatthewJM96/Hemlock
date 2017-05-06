#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec4 vertexColour;
out vec2 textureCoord;

void main(){

	gl_Position = proj * view * model * vec4(position, 1.0f);
	//gl_Position = vec4(position, 1.0f);
	
	vertexColour = colour;
	textureCoord = texCoord;
}

