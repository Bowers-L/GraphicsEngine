#shader vertex
#version 330 core

layout(location = 0) in vec4 v_Pos;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main() {
	gl_Position = u_Proj * u_View * u_Model * v_Pos;
};

#shader fragment
#version 330 core

out vec4 OutColor;

void main() {
	OutColor = vec4(1.0);
}