#include "TestModel.h"
#include "imgui/imgui.h"

#define WIDTH 640
#define HEIGHT 480

namespace test {
	TestModel::TestModel()
		: model("res/models/nanosuit/nanosuit.obj"),
		mModel(glm::mat4(1.0f)),
		mView(glm::mat4(1.0f)),
		mProj(glm::mat4(1.0f)),
		camera(_3DCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH)),

		light0(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
		light1(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f))
	{
		mView = camera.getViewMatrix();
		mProj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

		//shaders
		shader = std::make_unique<Shader>("res/shaders/Model.shader");
		shader->setUniformMat4f("u_Proj", mProj);
		LightShader = std::make_unique<Shader>("res/shaders/Light.shader");
		LightShader->setUniformMat4f("u_Proj", mProj);

		//setting uniforms for lights

		//directional light
		shader->setUniform4f("u_DirLight.direction", 0.0f, -1.0f, 0.0f, 0.0f);
		shader->setUniform4f("u_DirLight.ambient", 0.2f, 0.2f, 0.2f, 1.0f);
		shader->setUniform4f("u_DirLight.diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
		shader->setUniform4f("u_DirLight.specular", 0.5f, 0.5f, 0.5f, 1.0f);

		//point lights
		shader->setUniform4f("u_PointLights[0].position", light0.m_Position.x, light0.m_Position.y, light0.m_Position.z, 1.0f);
		shader->setUniform4f("u_PointLights[0].ambient", 0.2f, 0.2f, 0.2f, 1.0f);
		shader->setUniform4f("u_PointLights[0].diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
		shader->setUniform4f("u_PointLights[0].specular", 0.5f, 0.5f, 0.5f, 1.0f);
		shader->setUniform1f("u_PointLights[0].attenConst", 1.0f);
		shader->setUniform1f("u_PointLights[0].attenLinear", 0.022);
		shader->setUniform1f("u_PointLights[0].attenQuad", .0019);

		shader->setUniform4f("u_PointLights[1].position", light1.m_Position.x, light1.m_Position.y, light1.m_Position.z, 1.0f);
		shader->setUniform4f("u_PointLights[1].ambient", 0.2f, 0.2f, 0.2f, 1.0f);
		shader->setUniform4f("u_PointLights[1].diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
		shader->setUniform4f("u_PointLights[1].specular", 0.5f, 0.5f, 0.5f, 1.0f);
		shader->setUniform1f("u_PointLights[1].attenConst", 1.0f);
		shader->setUniform1f("u_PointLights[1].attenLinear", 0.022);
		shader->setUniform1f("u_PointLights[1].attenQuad", .0019);
	}

	TestModel::~TestModel()
	{
	}

	void TestModel::onUpdate(float deltaTime)
	{

		fps = 1 / deltaTime;

		mView = camera.getViewMatrix();
	}
	void TestModel::onRender()
	{
		Renderer::setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		Renderer::clear();

		//setting MVP matrices
		shader->setUniformMat4f("u_View", mView);
		LightShader->setUniformMat4f("u_View", mView);
		shader->setUniform4f("u_ViewPos", camera.Position.x, camera.Position.y, camera.Position.z, 1.0f);

		mModel = glm::mat4(1.0f);
		mModel = glm::translate(mModel, glm::vec3(0.0f, 0.0f, 0.0f));
		mModel = glm::scale(mModel, glm::vec3(0.2f, 0.2f, 0.2f));
		shader->setUniformMat4f("u_Model", mModel);

		//draw
		model.Draw(*shader);

		light0.draw(*LightShader);
		light1.draw(*LightShader);
	}
	void TestModel::onImGuiRender()
	{
		ImGui::SliderFloat("mouse sensitivity", &camera.MouseSensitivity, 0.0f, 1.0f);
		ImGui::SliderFloat("movement speed", &camera.MovementSpeed, 0.0f, 25.0f);

		ImGui::Text(std::string("fps: ").append(std::to_string(fps)).c_str());
	}

	void TestModel::processInput(GLFWwindow* window, float deltaTime) {

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		camera.processMouseMovement(xpos - WIDTH / 2, HEIGHT / 2 - ypos);

		int up = glfwGetKey(window, GLFW_KEY_W);
		int down = glfwGetKey(window, GLFW_KEY_S);
		int left = glfwGetKey(window, GLFW_KEY_A);
		int right = glfwGetKey(window, GLFW_KEY_D);

		if (up == GLFW_PRESS) {
			camera.processKeyboard(FORWARD, deltaTime);
		}
		if (down == GLFW_PRESS) {
			camera.processKeyboard(BACK, deltaTime);
		}
		if (left == GLFW_PRESS) {
			camera.processKeyboard(LEFT, deltaTime);
		}
		if (right == GLFW_PRESS) {
			camera.processKeyboard(RIGHT, deltaTime);
		}
	}
}