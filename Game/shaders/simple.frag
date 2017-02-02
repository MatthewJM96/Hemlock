#version 330 core

// Ouput data
out vec4 color;

void main()
{

	// Output color = red 
	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//color = vec3((abs(sin(gl_FragCoord.x) - cos(gl_FragCoord.y + gl_FragCoord.w))) * 0.5, (abs(cos(gl_FragCoord.w) + sin(gl_FragCoord.x - gl_FragCoord.z)) * 0.5), gl_FragCoord.z);
}