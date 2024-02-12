#version 400

// Default colour output
layout ( location = 0 ) out vec4 out_Colour;
in vec4 ex_BoneId;

void main(void)
{
	vec3 color = vec3(1);
	if (int(ex_BoneId.x) % 6 == 0)
	{
		color = vec3(1, 0, 0);
	}
	if (int(ex_BoneId.x) % 6 == 1)
	{
		color = vec3(1, 1, 0);
	}
	if (int(ex_BoneId.x) % 6 == 2)
	{
		color = vec3(0, 1, 0);
	}
	if (int(ex_BoneId.x) % 6 == 3)
	{
		color = vec3(1, 0, 1);
	}
	if (int(ex_BoneId.x) % 6 == 4)
	{
		color = vec3(0, 0, 1);
	}
	if (int(ex_BoneId.x) % 6 == 5)
	{
		color = vec3(0, 1, 1);
	}

	out_Colour = vec4(color, 1);
}
