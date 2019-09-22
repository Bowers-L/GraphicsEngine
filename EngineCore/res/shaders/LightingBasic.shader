#shader vertex
#version 330 core

layout(location = 0) in vec4 v_Pos;
layout(location = 1) in vec3 v_Normal;

out vec4 FragPos;
out vec3 Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main() {
	gl_Position = u_Proj * u_View * u_Model * v_Pos;

	FragPos = u_Model * v_Pos;
	Normal = mat3(transpose(inverse(u_Model))) * v_Normal;
};

#shader fragment
#version 330 core

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

struct Light {
	vec4 position;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

in vec4 FragPos;
in vec3 Normal;

out vec4 OutColor;

uniform Material u_Material;
uniform Light u_Light;

uniform vec4 u_LightPos;
uniform vec4 u_ViewPos;

void main() {

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(vec3(u_Light.position - FragPos));

	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(vec3(u_ViewPos - FragPos));

	float diffuseFactor = max(dot(norm, lightDir), 0.0f);
	float specularFactor = pow(max(dot(reflectDir, viewDir), 0.0f), u_Material.shininess);

	vec4 ambientColor = u_Material.ambient * u_Light.ambient;
	vec4 diffuseColor = diffuseFactor * u_Material.diffuse * u_Light.diffuse;
	vec4 specularColor = specularFactor * u_Material.specular * u_Light.specular;
	
	OutColor = (ambientColor + diffuseColor + specularColor);
}