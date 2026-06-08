#triangle vertex shader
#version 410 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};

#triangle fragment shader
#version 410 core	

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
};