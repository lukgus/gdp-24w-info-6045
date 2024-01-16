#version 400

layout(location=0) in vec4 in_Position;

out vec4 ex_Position;			
out vec4 ex_PositionWorld;
out vec4 ex_Normal;
out vec4 ex_UV_x2;

uniform mat4 ModelMatrix;	
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
	mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	gl_Position = MVP * in_Position;
}
