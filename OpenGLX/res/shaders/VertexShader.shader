#version 330 core

// Input vertex data, different for all executions of this shader.
// Notice that the "location" equals the location in glVertexAttribPointer
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main() 
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

	UV = vertexUV;
}