#include "stdafx.h"
#include "window.h"

/* Print the version of openGL and GLSL. */
void print_versions()
{
	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	//If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

/* Setup glew. */
void setup_glew()
{
	// Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		glfwTerminate();
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

/* Setup OpenGL settings. */
void setup_opengl_settings()
{
	// Setup GLEW
	setup_glew();

	// Enable multisampling
	glEnable(GL_MULTISAMPLE);

	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);

	// Related to shaders and z value comparisons for the depth buffer
	glDepthFunc(GL_LEQUAL);

	// Set polygon drawing mode to fill front and back of each polygon
	// You can also use the parameter of GL_LINE instead of GL_FILL to see wireframes
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Disable backface culling to render both sides of polygons
	glDisable(GL_CULL_FACE);

	// Set clear color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

/* Error callback to write error to the console. */
void error_callback(int error, const char* description)
{
	// Print error
	fputs(description + error, stderr);
}

/* Setup callback to handle openGL setup. */
void setup_callbacks(GLFWwindow *pWindow)
{
	// Set the error callback
	glfwSetErrorCallback(error_callback);

	// Set the window resize callback
	glfwSetFramebufferSizeCallback(pWindow, Window::resize_callback);

	// Set the key callback
	glfwSetKeyCallback(pWindow, Window::key_callback);

	// Set the mouse callback
	glfwSetCursorPosCallback(pWindow, Window::cursor_pos_callback);
	glfwSetMouseButtonCallback(pWindow, Window::cursor_button_callback);
	glfwSetScrollCallback(pWindow, Window::cursor_scroll_callback);
}

// Main function
int main(void)
{
	// Create the GLFW window
	GLFWwindow* pWindow = Window::create_window(800, 600);

	// Print OpenGL and GLSL versions
	print_versions();

	// Setup OpenGL settings, including lighting, materials, etc.
	setup_opengl_settings();

	// Setup callbacks
	setup_callbacks(pWindow);

	// Initialize objects/pointers for rendering
	Window::Start(); // This is a call in window which intiializes everything else.

	// Loop while GLFW window should stay open
	while (!glfwWindowShouldClose(pWindow))
	{
		// Main render display callback. Rendering of objects is done here.
		Window::display_callback(pWindow);

		// Idle callback. Updating objects, etc. can be done here.
		Window::idle_callback();
	}

	Window::Stop();

	// Destroy the window
	glfwDestroyWindow(pWindow);

	// Terminate GLFW
	glfwTerminate();

	// Proper exit
	exit(EXIT_SUCCESS);
}