#include <application.h>

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
	shaderManager.useShaderProgram();
	std::cout << "Shader program created successfully!" << std::endl;
	// ------------------------------------------------------------------------------->>

	float vertices[] = {
		// positions			// colors
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	printOpenGLInfo();

	while (!glfwWindowShouldClose(window) && isRunning)
	{
		fpsCalculate();
		processInput();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderManager.useShaderProgram();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

// Old implementation ---------------------------------------------->>

//class Application
//{
//private:
//	GLFWwindow* window;
//	int width;
//	int height;
//	const char* title;
//	bool isRunning;
//public:
//	Application(int width, int height, const char* title)
//	{
//		this->width = width;
//		this->height = height;
//		this->title = title;
//		this->isRunning = true;
//	}
//
//	~Application()
//	{
//		glfwTerminate();
//	}
//
//	void fpsCalculate()
//	{
//		static double previousSeconds = glfwGetTime();
//		static int frameCount;
//		double elapsedSeconds;
//		double currentSeconds = glfwGetTime();
//
//		elapsedSeconds = currentSeconds - previousSeconds;
//
//		if (elapsedSeconds > 0.25)
//		{
//			previousSeconds = currentSeconds;
//			double fps = (double)frameCount / elapsedSeconds;
//			double msPerFrame = 1000.0 / fps;
//
//			char tmp[128];
//			sprintf(tmp, "%s @ fps: %.2f, ms/frame: %.2f", title, fps, msPerFrame);
//			glfwSetWindowTitle(window, tmp);
//
//			frameCount = 0;
//		}
//
//		frameCount++;
//	}
//
//	void processInput()
//	{
//		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		{
//			isRunning = false;
//		}
//	}
//
//	void printOpenGLInfo()
//	{
//		const GLubyte* renderer = glGetString(GL_RENDERER);
//		const GLubyte* version = glGetString(GL_VERSION);
//		std::cout << "Renderer: " << renderer << std::endl;
//		std::cout << "OpenGL version supported: " << version << std::endl;
//	}
//
//	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//	{
//		glViewport(0, 0, width, height);
//	}
//
//	void run()
//	{
//		if (!glfwInit())
//		{
//			std::cout << "Failed to initialize GLFW" << std::endl;
//			return;
//		}
//
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//		window = glfwCreateWindow(width, height, title, NULL, NULL);
//		if (!window)
//		{
//			std::cout << "Failed to create window" << std::endl;
//			glfwTerminate();
//			return;
//		}
//
//		glfwMakeContextCurrent(window);
//		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//		{
//			std::cout << "Failed to initialize GLAD" << std::endl;
//			return;
//		}
//
//		printOpenGLInfo();
//
//		while (!glfwWindowShouldClose(window) && isRunning)
//		{
//			fpsCalculate();
//			processInput();
//			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//			glClear(GL_COLOR_BUFFER_BIT);
//			glfwSwapBuffers(window);
//			glfwPollEvents();
//		}
//
//	}
//};
