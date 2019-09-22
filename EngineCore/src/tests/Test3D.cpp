#include "Test3D.h"

#include "imgui/imgui.h"

#define WIDTH 640
#define HEIGHT 480

namespace test {

	Test3D::Test3D()
		: Model(glm::mat4(1.0f)),
		View(glm::mat4(1.0f)),
		Proj(glm::mat4(1.0f)),
		time(0.00f),
		radius(5.0f),
		Cam(_3DCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH))
	{
		glEnable(GL_DEPTH_TEST);

		//Vertex Data

		float vertices[] = {
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,	//bottom left far 0
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f,	//bottom right far 1
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f,	//top right far 2
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f,	//top left far 3

		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	//bottom left near 4
		 0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,	//bottom right near 5
		 0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	//top right near 6
		-0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,	//top left near 7

		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	//bottom left far 8 *
		-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,	//bottom left near 9
		-0.5f,  0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top left near 10
		-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,	//top left far 11 *

		 0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	//bottom right near 12
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,	//bottom right far 13 *
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,	//top right far 14 *
		 0.5f,  0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,	//top right near 15

		-0.5f,  0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,	//top left near 16
		 0.5f,  0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//top right near 17
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,	//top right far 18 *
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	//top left far 19 *

		-0.5f,  -0.5f, 0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,	//bottom left near 20
		 0.5f,  -0.5f, 0.5f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f,	//bottom right near 21
		 0.5f,  -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f,	//bottom right far 22 *
		-0.5f,  -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,	//bottom left far 23 *
		};

		//Index data

		unsigned int indices[36];
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				switch (j) {
				case 0:
				case 1:
				case 2:
					indices[i * 6 + j] = i * 4 + j;
					break;
				case 3:
				case 4:
					indices[i * 6 + j] = i * 4 + j - 1;
					break;
				case 5:
					indices[i * 6 + j] = i * 4;
					break;
				}
			}
		}

		//light positions
		PointLightPositions[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		PointLightPositions[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		PointLightPositions[2] = glm::vec4(2.0f, 0.0f, 0.0f, 1.0f);
		PointLightPositions[3] = glm::vec4(3.0f, 0.0f, 0.0f, 1.0f);

		//Vertex Buffer + layout

		CubeVB = std::make_unique<VertexBuffer>(vertices, 24 * 8 * sizeof(float), GL_STATIC_DRAW);
		VertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(3);
		layout.push<float>(2);

		//Index Buffer

		CubeIB = std::make_unique<IndexBuffer>(indices, 36, GL_STATIC_DRAW);

		//Vertex Arrays

		LightVA = std::make_unique<VertexArray>();
		LightVA->addBuffer(*CubeVB, layout);

		ObjVA = std::make_unique<VertexArray>();
		ObjVA->addBuffer(*CubeVB, layout);

		//Texture

		ObjDiffuseMap = std::make_unique<Texture>("texture_diffuse", "res/textures/container2.png");
		ObjSpecularMap = std::make_unique<Texture>("texture_specular", "res/textures/container2_specular.png");
		ObjEmissionMap = std::make_unique<Texture>("texture_emission", "res/textures/matrix.jpg");
		
		//Shaders

		LightShader = std::make_unique<Shader>("res/shaders/Light.shader");
		ObjShader = std::make_unique<Shader>("res/shaders/LightingTexture.shader");

		//material
		ObjShader->setUniform1i("u_Material.diffuse", 0);
		ObjShader->setUniform1i("u_Material.specular", 1);
		ObjShader->setUniform1i("u_Material.emission", 2);
		ObjShader->setUniform1f("u_Material.shininess", 4);
		
		// directional light
		ObjShader->setUniform4f("u_DirLight.direction", -0.2f, -1.0f, -0.3f, 0.0f);
		ObjShader->setUniform4f("u_DirLight.ambient", 0.05f, 0.05f, 0.05f, 1.0f);
		ObjShader->setUniform4f("u_DirLight.diffuse", 0.4f, 0.4f, 0.4f, 1.0f);
		ObjShader->setUniform4f("u_DirLight.specular", 0.5f, 0.5f, 0.5f, 1.0f);
		// point light 1
		ObjShader->setUniform4f("u_PointLights[0].position", PointLightPositions[0].x, PointLightPositions[0].y, PointLightPositions[0].z, 1.0f);
		ObjShader->setUniform4f("u_PointLights[0].ambient", 0.05f, 0.05f, 0.05f, 1.0f);
		ObjShader->setUniform4f("u_PointLights[0].diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
		ObjShader->setUniform4f("u_PointLights[0].specular", 1.0f, 1.0f, 1.0f, 1.0f);
		ObjShader->setUniform1f("u_PointLights[0].attenConstant", 1.0f);
		ObjShader->setUniform1f("u_PointLights[0].attenLinear", 0.022);
		ObjShader->setUniform1f("u_PointLights[0].attenQuadratic", .0019);
		// point light 2
		ObjShader->setUniform4f("u_PointLights[1].position", PointLightPositions[1].x, PointLightPositions[1].y, PointLightPositions[1].z, 1.0f);
		ObjShader->setUniform4f("u_PointLights[1].ambient", 0.05f, 0.05f, 0.05f, 1.0f);
		ObjShader->setUniform4f("u_PointLights[1].diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
		ObjShader->setUniform4f("u_PointLights[1].specular", 1.0f, 1.0f, 1.0f, 1.0f);
		ObjShader->setUniform1f("u_PointLights[1].attenConstant", 1.0f);
		ObjShader->setUniform1f("u_PointLights[1].attenLinear", 0.022);
		ObjShader->setUniform1f("u_PointLights[1].attenQuadratic", .0019);
		// point light 3
		ObjShader->setUniform4f("u_PointLights[2].position", PointLightPositions[2].x, PointLightPositions[2].y, PointLightPositions[2].z, 1.0f);
		ObjShader->setUniform4f("u_PointLights[2].ambient", 0.05f, 0.05f, 0.05f, 1.0f);
		ObjShader->setUniform4f("u_PointLights[2].diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
		ObjShader->setUniform4f("u_PointLights[2].specular", 1.0f, 1.0f, 1.0f, 1.0f);
		ObjShader->setUniform1f("u_PointLights[2].attenConstant", 1.0f);
		ObjShader->setUniform1f("u_PointLights[2].attenLinear", 0.022);
		ObjShader->setUniform1f("u_PointLights[2].attenQuadratic", .0019);
		// point light 4
		ObjShader->setUniform4f("u_PointLights[3].position", PointLightPositions[3].x, PointLightPositions[3].y, PointLightPositions[3].z, 1.0f);
		ObjShader->setUniform4f("u_PointLights[3].ambient", 0.05f, 0.05f, 0.05f, 1.0f);
		ObjShader->setUniform4f("u_PointLights[3].diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
		ObjShader->setUniform4f("u_PointLights[3].specular", 1.0f, 1.0f, 1.0f, 1.0f);
		ObjShader->setUniform1f("u_PointLights[3].attenConstant", 1.0f);
		ObjShader->setUniform1f("u_PointLights[3].attenLinear", 0.022);
		ObjShader->setUniform1f("u_PointLights[3].attenQuadratic", .0019);
		// spotLight
		ObjShader->setUniform4f("u_SpotLight.position", Cam.Position.x, Cam.Position.y, Cam.Position.z, 1.0f);
		ObjShader->setUniform4f("u_SpotLight.direction", Cam.getFront().x, Cam.getFront().y, Cam.getFront().z, 1.0f);
		ObjShader->setUniform4f("u_SpotLight.ambient", 0.2f, 0.2f, 0.2f, 1.2f);
		ObjShader->setUniform4f("u_SpotLight.diffuse", 0.5f, 0.5f, 0.5f, 1.0f);
		ObjShader->setUniform4f("u_SpotLight.specular", 1.0f, 1.0f, 1.0f, 1.0f);
		ObjShader->setUniform1f("u_SpotLight.attenConstant", 1.0f);
		ObjShader->setUniform1f("u_SpotLight.attenConstant", 0.35);
		ObjShader->setUniform1f("u_SpotLight.attenConstant", 0.44);
		ObjShader->setUniform1f("u_SpotLight.innerCutoff", glm::cos(glm::radians(12.5f)));
		ObjShader->setUniform1f("u_SpotLight.outerCutoff", glm::cos(glm::radians(15.0f)));

		/*
		TexShader = std::make_unique<Shader>("res/shaders/Texture.shader");
		TexShader->setUniformMat4f("u_MVP", MVP);
		TexShader->setUniform1i("u_Texture", 0);
		*/

		//MVP

		Proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		View = Cam.getViewMatrix();
	}
	Test3D::~Test3D()
	{
	}
	void Test3D::onUpdate(float deltaTime)
	{
		time += deltaTime;
		fps = (int) (1 / deltaTime);

		//LightPos = glm::vec3(2 * sin(glm::radians(time * 5)), cos(glm::radians(time * 50)), sin(glm::radians(time * 50)));

		/* Camera Animation

		glm::vec3 pos = glm::vec3(radius * cos(glm::radians(theta)), radius * sin(glm::radians(theta)), radius * sin(glm::radians(theta)));
		float yaw = theta + 180.0f;
		float pitch = -45.0f * sin(glm::radians(theta));

		Cam.setPosRot(pos, yaw, pitch);
		*/

		View = Cam.getViewMatrix();
	}
	void Test3D::onRender()
	{

		float vertices1[] = {
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,  0.0f, 0.0f,	//bottom left far 0
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,  1.0f, 0.0f,	//bottom right far 1
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,  1.0f, 1.0f,	//top right far 2
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, -1.0f,  0.0f, 1.0f,	//top left far 3

		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	//bottom left near 4
		 0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   1.0f, 0.0f,	//bottom right near 5
		 0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   1.0f, 1.0f,	//top right near 6
		-0.5f,  0.5f, 0.5f,		0.0f, 0.0f, 1.0f,   0.0f, 1.0f,	//top left near 7

		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,  0.0f, 0.0f,		//bottom left far 8 *
		-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		//bottom left near 9
		-0.5f,  0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		//top left near 10
		-0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		//top left far 11 *

		 0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		//bottom right near 12
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		//bottom right far 13 *
		 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		//top right far 14 *
		 0.5f,  0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		//top right near 15

		-0.5f,  0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,	//top left near 16
		 0.5f,  0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	//top right near 17
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,	//top right far 18 *
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,	//top left far 19 *

		-0.5f,  -0.5f, 0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f,	//bottom left near 20
		 0.5f,  -0.5f, 0.5f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f,	//bottom right near 21
		 0.5f,  -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f,	//bottom right far 22 *
		-0.5f,  -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,	//bottom left far 23 *
		};

		//setting the MVP matrix
		Renderer::setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		Renderer::clear();


		for (int i = 0; i < 10; i++) {

			if (time >= 5) {
				PRINTLN("Doing the thing");
				CubeVB->updateBuffer(0, 24 * 8 * sizeof(float), (void *) vertices1);
			}
			//object model matrix
			Model = glm::mat4(1.0f);
			Model = glm::translate(Model, glm::vec3((i-1) * 2.0f, -i * 2.0f, 0.0f));

			ObjShader->setUniformMat4f("u_Model", Model);
			ObjShader->setUniformMat4f("u_View", View);
			ObjShader->setUniformMat4f("u_Proj", Proj);

			ObjShader->setUniform4f("u_SpotLight.position", Cam.Position.x, Cam.Position.y, Cam.Position.z, 1.0f);
			ObjShader->setUniform4f("u_SpotLight.direction", Cam.getFront().x, Cam.getFront().y, Cam.getFront().z, 0.0f);
			ObjShader->setUniform4f("u_ViewPos", Cam.Position.x, Cam.Position.y, Cam.Position.z, 1.0f);

			ObjDiffuseMap->bind(0);
			ObjSpecularMap->bind(1);
			ObjEmissionMap->bind(2);

			Renderer::draw(*ObjVA, *CubeIB, *ObjShader);


		}

		//Light model matrix
		for (int i = 0; i < 4; i++) {
			Model = glm::mat4(1.0f);
			Model = glm::translate(Model, PointLightPositions[i]);
			Model = glm::scale(Model, glm::vec3(0.2f));

			LightShader->setUniformMat4f("u_Model", Model);
			LightShader->setUniformMat4f("u_View", View);
			LightShader->setUniformMat4f("u_Proj", Proj);
			Renderer::draw(*LightVA, *CubeIB, *LightShader);
		}

	}

	void Test3D::onImGuiRender()
	{
		ImGui::SliderFloat("radius", &radius, 0.0f, 20.0f);
		ImGui::SliderFloat("mouse sensitivity", &Cam.MouseSensitivity, 0.0f, 1.0f);
		ImGui::SliderFloat("movement speed", &Cam.MovementSpeed, 0.0f, 5.0f);

		
		ImGui::Text(std::string("fps: ").append(std::to_string(fps)).c_str());
	}

	void Test3D::processInput(GLFWwindow* window, float deltaTime) {
	
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		Cam.processMouseMovement(xpos - WIDTH / 2, HEIGHT / 2 - ypos);

		int up = glfwGetKey(window, GLFW_KEY_W);
		int down = glfwGetKey(window, GLFW_KEY_S);
		int left = glfwGetKey(window, GLFW_KEY_A);
		int right = glfwGetKey(window, GLFW_KEY_D);

		if (up == GLFW_PRESS) {
			Cam.processKeyboard(FORWARD, deltaTime);
		}
		if (down == GLFW_PRESS) {
			Cam.processKeyboard(BACK, deltaTime);
		}
		if (left == GLFW_PRESS) {
			Cam.processKeyboard(LEFT, deltaTime);
		}
		if (right == GLFW_PRESS) {
			Cam.processKeyboard(RIGHT, deltaTime);
		}
	}
}