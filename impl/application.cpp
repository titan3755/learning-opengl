#include <application.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Application::Application(int width, int height, const char* title)
{
	this->window = nullptr;
	this->width = width;
	this->height = height;
	this->title = title;
	this->isRunning = true;
}

Application::~Application()
{
	glfwTerminate();
}

void Application::fpsCalculate()
{
	static double previousSeconds = glfwGetTime();
	static int frameCount;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		char tmp[128];
		sprintf_s(tmp, "%s @ fps: %.2f, ms/frame: %.2f", title, fps, msPerFrame);
		glfwSetWindowTitle(window, tmp);

		frameCount = 0;
	}

	frameCount++;
}

void Application::processInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		isRunning = false;
	}
}

void Application::printOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	const GLubyte* vendor = glGetString(GL_VENDOR);

	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported: " << version << std::endl;
	std::cout << "GLSL version: " << glslVersion << std::endl;
	std::cout << "Vendor: " << vendor << std::endl;
	std::cout << "Reached application loop..." << std::endl;
	std::cout << "OpenGL initialized successfully!" << std::endl;
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Application::run()
{
	std::cout << "Running application and initializing OpenGL..." << std::endl;

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ------------------------------------------------------------------------------->>
	// shader manager implementation
	std::cout << "Creating shader program..." << std::endl;
	bool success = true;
	ShaderManager shaderManager;
	success = shaderManager.createShaderProgram();
	if (!success)
	{
		std::cerr << "Failed to create shader program!" << std::endl;
		return;
	}
	success = shaderManager.compileShaders();
	if (!success)
	{
		std::cerr << "Failed to compile shaders!" << std::endl;
		return;
	}
	std::cout << "Shader program created successfully!" << std::endl;
	// ------------------------------------------------------------------------------->>


	float vertices[] = {
		// for rectangle
		// positions		// colors
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,		// top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,	// bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,	// top left
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	printOpenGLInfo();
	shaderManager.useShaderProgram();

	// ------------------------------------------------------------------------------->>

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(0.01f), glm::vec3(0.0f, 0.0f, 0.01f));
	transform = glm::translate(transform, glm::vec3(0.01f, -0.01f, 0.0f));
	unsigned int transformLoc = glGetUniformLocation(shaderManager.getShaderProgram(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	while (!glfwWindowShouldClose(window) && isRunning)
	{
		fpsCalculate();
		processInput();
		glClearColor(0.8f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderManager.useShaderProgram();
		transform = glm::rotate(transform, glm::radians(0.01f), glm::vec3(0.0f, 0.0f, 0.01f));
		transform = glm::translate(transform, glm::vec3(0.01f, -0.01f, 0.0f));
		transformLoc = glGetUniformLocation(shaderManager.getShaderProgram(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}