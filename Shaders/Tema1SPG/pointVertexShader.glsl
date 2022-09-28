#version 330

layout(location = 0) in vec3 in_position;		

uniform mat4 projection_matrix, view_matrix, model_matrix;

void main()
{
	gl_Position = projection_matrix * view_matrix * model_matrix*vec4(in_position, 1); 
}
