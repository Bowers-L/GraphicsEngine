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
}

#shader fragment
#version 330 core

#define NUM_TEXTURES 8
#define NUM_POINT_LIGHTS 2

struct Material {
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
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

	float attenConst;
	float attenLinear;
	float attenQuad;
};

struct SpotLight {
	vec4 position;

	vec4 direction;
	float outerCutoff;
	float innerCutoff;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	
	float attenConst;
	float attenLinear;
	float attenQuad;
};

in vec4 FragPos;
in vec4 Normal;
in vec2 TexCoords;

out vec4 OutColor;

uniform Material u_Material;

uniform DirectionalLight u_DirLight;
uniform PointLight u_PointLights[NUM_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

uniform vec4 u_ViewPos;

vec4 calcDirLight(DirectionalLight light, vec4 viewDir, vec4 normal);
vec4 calcPointLight(PointLight light, vec4 viewDir, vec4 normal);
vec4 calcSpotLight(SpotLight light, vec4 viewDir, vec4 normal);

void main() {
	vec4 viewDir = normalize(FragPos - u_ViewPos);
	vec4 normal = normalize(Normal);

	vec4 color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//vec4 color = calcDirLight(u_DirLight, viewDir, normal);

	
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		color += calcPointLight(u_PointLights[i], viewDir, normal);
	}
	


	/*
	if (u_SpotLight) {
		color += calcSpotLight(u_SpotLight, viewDir, normal);
	}
	*/

	OutColor = color;
}


/*
viewDir - normalized, coming from view position to fragment position
normal - normalized
*/


vec4 calcDirLight(DirectionalLight light, vec4 viewDir, vec4 normal) {
	vec4 lightDir = normalize(light.direction);

	//ambient color
	vec4 ambientColor = light.ambient * texture(u_Material.texture_diffuse0, TexCoords);

	//diffuse color
	float diffuseFactor = max(dot(-lightDir, normal), 0.0f);
	vec4 diffuseColor = diffuseFactor * light.diffuse * texture(u_Material.texture_diffuse0, TexCoords);

	//specular color

	vec4 reflectedDir = reflect(lightDir, normal);
	float specularFactor = pow(max(dot(-reflectedDir, viewDir), 0.0f), u_Material.shininess);
	vec4 specularColor = specularFactor * light.specular * texture(u_Material.texture_specular0, TexCoords);

	return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight(PointLight light, vec4 viewDir, vec4 normal) {
	vec4 lightDir = normalize(FragPos - light.position);

	//ambient color
	vec4 ambientColor = light.ambient * texture(u_Material.texture_diffuse0, TexCoords);

	//diffuse color
	float diffuseFactor = max(dot(-lightDir, normal), 0.0f);
	vec4 diffuseColor = diffuseFactor * light.diffuse * texture(u_Material.texture_diffuse0, TexCoords);

	//specular color

	vec4 reflectedDir = reflect(lightDir, normal);
	float specularFactor = pow(max(dot(-reflectedDir, viewDir), 0.0f), u_Material.shininess);
	vec4 specularColor = specularFactor * light.specular * texture(u_Material.texture_specular0, TexCoords);

	//attenuation
	float dist = length(FragPos - light.position);
	float attenuation = 1.0f / (light.attenConst + light.attenLinear * dist + light.attenQuad * dist * dist);

	return (ambientColor + diffuseColor + specularColor) * attenuation;
}

vec4 calcSpotLight(SpotLight light, vec4 viewDir, vec4 normal) {
	vec4 lightDir = normalize(FragPos - light.position);

	//ambient color
	vec4 ambientColor = light.ambient * texture(u_Material.texture_diffuse0, TexCoords);

	//diffuse color
	float diffuseFactor = max(dot(-lightDir, normal), 0.0f);
	vec4 diffuseColor = diffuseFactor * light.diffuse * texture(u_Material.texture_diffuse0, TexCoords);

	//specular color

	vec4 reflectedDir = reflect(lightDir, normal);
	float specularFactor = pow(max(dot(-reflectedDir, viewDir), 0.0f), u_Material.shininess);
	vec4 specularColor = specularFactor * light.specular * texture(u_Material.texture_specular0, TexCoords);

	//attenuation
	float dist = length(FragPos - light.position);
	float attenuation = 1.0f / (light.attenConst + light.attenLinear * dist + light.attenQuad * dist * dist);

	//intensity
	float cosTheta = dot(normalize(light.direction), lightDir);
	float intensity = (cosTheta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff);
	intensity = clamp(intensity, 0.0f, 1.0f);

	diffuseColor *= intensity;
	specularColor *= intensity;

	return (ambientColor + diffuseColor + specularColor) * attenuation;
}
