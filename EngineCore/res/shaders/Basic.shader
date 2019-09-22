#shader vertex
#version 330 core

layout(location = 0) in vec4 vertPos;

uniform mat4 u_MVP;

void main()
{
 gl_Position = u_MVP * vertPos;
};

#shader fragment
#version 330 core

out vec4 outColor;

uniform vec4 u_Color;

void main()
{
	outColor = u_Color;
};