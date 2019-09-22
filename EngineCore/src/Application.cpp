#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "util/Macros.h"

#include "Renderer.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexArray.h"
#include "buffers/IndexBuffer.h"

#include "Texture.h"
#include "Shader.h"

#include "tests/AllTests.h"
#include "tests/TestMenu.h"

#include <time.h>

/*
Textures:
1. Load image into CPU memory (format is important)
	Use stb_image:
	give filepath, output rgba buffer of pixels
2. Use OpenGL to send image data to the GPU (as a texture)
3. Modify the shader to read the texture memory and sample it
*/

#define WIDTH 640
#define HEIGHT 480

int main(void)
{
	GLFWwindow* window;

	/* Initialize GLFW */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	const char* glsl_version = "#version 330 core";

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Tasty Graphics", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);	//Basically the framerate

	/*Initialize GLAD*/
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		PRINTLN("Failed to initialize GLAD");
		return -1;
	}
	PRINTLN(glGetString(GL_VERSION));

	//Create context for ImGui and setup
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui::StyleColorsDark();

	//Creating a scope so that everything on the stack will be deleted before calling glfwTerminate()
	{

		//Tests

		test::Test* currentTest;
		test::TestMenu* menu = new test::TestMenu(currentTest);
		currentTest = menu;

		menu->addTest<test::TestClearColor>("Clear Color");
		menu->addTest<test::TestTexture2D>("Texture 2D");
		menu->addTest<test::TestBlending>("Blending");
		menu->addTest<test::Test3D>("3D");
		menu->addTest<test::TestModel>("Model");


		clock_t old_t = clock();
		clock_t t = clock() - old_t;
		while (!glfwWindowShouldClose(window))
		{
			old_t = clock();

			//Initialize imgui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			/* Render here */
			if (currentTest) {
				currentTest->onUpdate(t / (float) CLOCKS_PER_SEC);
				currentTest->onRender();

				ImGui::Begin("Name of Test");
				if (currentTest != menu && ImGui::Button("<-")) {
					delete currentTest;
					currentTest = menu;
				}
				currentTest->onImGuiRender();
				ImGui::End();
				currentTest->processInput(window, t / (float) CLOCKS_PER_SEC);
			}

			//Render imgui stuff
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			//Rendering happens on the back buffer, and the front buffer is what is displayed when this is called
			glfwSwapBuffers(window);

			/* Poll for and process events */
			//This is used if the window is implementing event handling
			glfwPollEvents();

			t = clock() - old_t;
		}

		if (currentTest != menu) {
			delete menu;
		}
		delete currentTest;
	}
	//delete stuff
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}