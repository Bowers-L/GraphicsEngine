#shader vertex
#version 330 core

layout(location = 0) in vec3 v_Pos;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoords;

out vec4 FragPos;
out vec4 Normal;
out vec2 TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main() {
	gl_Position = u_Proj * u_View * u_Model * vec4(v_Pos, 1.0f);

	FragPos = u_Model * vec4(v_Pos, 1.0f);
	Normal = vec4(mat3(transpose(inverse(u_Model))) * v_Normal, 1.0f);
	TexCoords = v_TexCoords;
};

/****************/

#shader fragment
#version 330 core

#define NUM_POINT_LIGHTS 4

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct DirectionalLight {
	vec4 direction;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct PointLight {
	vec4 position;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float attenConstant;
	float attenLinear;
	float attenQuadratic;

};

struct SpotLight {
	vec4 position;
	vec4 direction;
	float innerCutoff;
	float outerCutoff;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float attenConstant;
	float attenLinear;
	float attenQuadratic;

};

in vec4 FragPos;
in vec4 Normal;
in vec2 TexCoords;

out vec4 OutColor;

uniform vec4 u_ViewPos;
uniform Material u_Material;
uniform DirectionalLight u_DirLight;
uniform PointLight u_PointLights[NUM_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

vec4 CalcDirectionalLight(DirectionalLight light, vec4 normal, vec4 viewDir);
vec4 CalcPointLight(PointLight light, vec4 normal, vec4 fragPos, vec4 viewDir);
vec4 CalcSpotLight(SpotLight light, vec4 normal, vec4 fragPos, vec4 viewDir);

void main() {
	vec4 normal = normalize(Normal);
	vec4 viewDir = normalize(FragPos - u_ViewPos);

	vec4 FragColor = CalcDirectionalLight(u_DirLight, normal, viewDir);
	
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		FragColor += CalcPointLight(u_PointLights[i], normal, FragPos, viewDir);
	}

	FragColor += CalcSpotLight(u_SpotLight, normal, FragPos, viewDir);

	OutColor = FragColor;
}

//normal and viewDir expected to be normalized

vec4 CalcDirectionalLight(DirectionalLight light, vec4 normal, vec4 viewDir) {
	vec4 lightDir = normalize(light.direction);

	//ambient
	vec4 ambientColor = light.ambient * vec4(texture(u_Material.diffuse, TexCoords));

	//diffuse
	float diffFactor = max(dot(-lightDir, normal), 0.0f);
	vec4 diffuseColor = diffFactor * light.diffuse * vec4(texture(u_Material.diffuse, TexCoords));

	//specular
	vec4 reflectedDir = reflect(lightDir, normal);
	float specularFactor = pow(max(dot(-reflectedDir, viewDir), 0.0f), u_Material.shininess);
	vec4 specularColor = specularFactor * light.specular * vec4(texture(u_Material.specular, TexCoords));

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcPointLight(PointLight light, vec4 normal, vec4 fragPos, vec4 viewDir) {
	vec4 lightDir = normalize(fragPos - light.position);

	//ambient
	vec4 ambientColor = light.ambient * vec4(texture(u_Material.diffuse, TexCoords));

	//diffuse
	float diffFactor = max(dot(-lightDir, normal), 0.0f);
	vec4 diffuseColor = diffFactor * light.diffuse * vec4(texture(u_Material.diffuse, TexCoords));

	//specular
	vec4 reflectedDir = reflect(lightDir, normal);
	float specularFactor = pow(max(dot(-reflectedDir, viewDir), 0.0f), u_Material.shininess);
	vec4 specularColor = specularFactor * light.specular * vec4(texture(u_Material.specular, TexCoords));

	//calculating attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.attenConstant + light.attenLinear * distance + light.attenQuadratic * (distance * distance));

	ambientColor *= attenuation;
	diffuseColor *= attenuation;
	specularColor *= attenuation;

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcSpotLight(SpotLight light, vec4 normal, vec4 fragPos, vec4 viewDir) {
	vec4 lightDir = normalize(fragPos - light.position);

	//ambient
	vec4 ambientColor = light.ambient * vec4(texture(u_Material.diffuse, TexCoords));

	//diffuse
	float diffFactor = max(dot(-lightDir, normal), 0.0f);
	vec4 diffuseColor = diffFactor * light.diffuse * vec4(texture(u_Material.diffuse, TexCoords));

	//specular
	vec4 reflectedDir = reflect(lightDir, normal);
	float specularFactor = pow(max(dot(-reflectedDir, viewDir), 0.0f), u_Material.shininess);
	vec4 specularColor = specularFactor * light.specular * vec4(texture(u_Material.specular, TexCoords));

	//calculating attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.attenConstant + light.attenLinear * distance + light.attenQuadratic * (distance * distance));

	//calculating intensity
	float cosTheta = dot(normalize(light.direction), lightDir);
	float intensity = clamp((cosTheta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0f, 1.0f);

	ambientColor *= attenuation;
	diffuseColor *= attenuation * intensity;
	specularColor *= attenuation * intensity;

	return (ambientColor + diffuseColor + specularColor);
}