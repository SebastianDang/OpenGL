#include "window.h"
#include "group.h"

using namespace std;

//Set the title of the program. This will remain static.
const char* window_title = "The Beginning";

//---------- Global window definitions ----------//

//Define Mouse control status for idle, left hold, right hold.
#define IDLE 0
#define LEFT_HOLD 1
#define RIGHT_HOLD 2

//---------- Global window properties ----------//

//Window size properties.
int Window::width;//Width of the window (This will be set in window_resize callback).
int Window::height;//Height of the window (This will be set in window_resize callback).
//Mouse properties.
int Window::mouse_status = IDLE;//Define the mouse status for any clicks.
glm::vec3 Window::lastPoint;//Last point clicked.
glm::vec3 Window::curPoint;//Current point clicked.
//Camera properties.
glm::vec3 Window::camera_pos = glm::vec3(0.0f, 0.0f, 20.0f);//Default.
glm::vec3 Window::camera_look_at = glm::vec3(0.0f, 0.0f, 0.0f);//Default.
glm::vec3 Window::camera_up = glm::vec3(0.0f, 1.0f, 0.0f);//Default. 
//Matrix Coordinate Transformation properties.
glm::mat4 Window::P;//Perspective.
glm::mat4 Window::V;//View.
//Frame Time calculation.
float Window::lastFrameTime;//Last frame time recorded.
float Window::delta;//The time since the last frame time, from the current frame time.

//---------- Components that used to create the scene ----------//

//Define any cameras here.

//Define any objects here.

//Define any environment variables here. We should always have the skybox!

//Define any shaders here.
GLint shaderProgram;

/* Initialize any objects in the scene here. */
void Window::initialize_objects()
{
	//Create shaders.
	shaderProgram = LoadShaders("../shader.vert", "../shader.frag");
}

/* Deconstructor, deletes all initialized objects for a proper cleanup. */
void Window::clean_up()
{
	//Delete shaders.
	glDeleteProgram(shaderProgram);
}

/* Create window is called to initialize the window. */
GLFWwindow* Window::create_window(int width, int height)
{
	//Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	//4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	//Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	//Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	//Make the context of the window
	glfwMakeContextCurrent(window);

	//Set swap interval to 1
	glfwSwapInterval(1);

	//Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	//Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	//Set the initial frame time.
	Window::lastFrameTime = (float)glfwGetTime();

	return window;
}

/* Window callback function on a resize. Resets the perspective and view for proper coordinate transformation. */
void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(Window::camera_pos, Window::camera_look_at, Window::camera_up);
	}
}

/* Idle callback function. This will call itself repeatedly as long as the program is running. */
void Window::idle_callback()
{
	//Calculate the frame time.
	float currentFrameTime = (float)glfwGetTime();//Get the current time.
	Window::delta = (currentFrameTime - Window::lastFrameTime);//Calculate the change from this frame time, to the old frame time.
	Window::lastFrameTime = currentFrameTime;//Record the (new per calculation) old frame time.

}

/* Display callback function. Whenever contents need to be redisplayed, this will be called. We render, or redraw any part of the scene here. */
void Window::display_callback(GLFWwindow* window)
{
	//Clear the color and depth buffers
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw the entire scene.
	Window::drawScene();
	
	//Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	//Swap buffers
	glfwSwapBuffers(window);
}

/* Perform any draw methods here. We handle this separately to avoid changing the swap buffer in display_callback. */
void Window::drawScene()
{
	//Use the shader of programID
	glUseProgram(shaderProgram);
	//Render the objects

}

/* Handle Key input. */
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//---------- Any global key definitions for holds and releases ----------//
	//Define shift keys for capital letters.
	int Lshift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	int Rshift = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);
	//Define movement keys.
	int wKey = glfwGetKey(window, GLFW_KEY_W);
	int aKey = glfwGetKey(window, GLFW_KEY_A);
	int sKey = glfwGetKey(window, GLFW_KEY_S);
	int dKey = glfwGetKey(window, GLFW_KEY_D);


	//---------- Any global keys presses that affect the window directly ----------//
	//Check for a single key press (Not holds)
	if (action == GLFW_PRESS)
	{
		//Check if escape was pressed.
		if (key == GLFW_KEY_ESCAPE)
		{
			//Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_0) {
		}
		if (key == GLFW_KEY_1) {
		}
		if (key == GLFW_KEY_2) {
		}
		if (key == GLFW_KEY_3) {
		}
		if (key == GLFW_KEY_4) {
		}
	}
}

/* Handle mouse movement input. */
void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Update current mouse position.
	glfwGetCursorPos(window, &xpos, &ypos);
	//Get current mouse position.
	Window::curPoint = glm::vec3(xpos, ypos, 0.0f);

	//On left drag, we perform rotations. Relative to the object.
	if (Window::mouse_status == LEFT_HOLD)
	{

	}
	//On right drag, we perform translations. Relative to the object.
	if (Window::mouse_status == RIGHT_HOLD)
	{

	}

}

/* Handle mouse button input. This handles the status if left button or right button was clicked and held. */
void Window::cursor_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
	//Define left and right clicks.
	int left_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int right_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	//Left click hold will save the position that the mouse was clicked and save it.
	if (left_click == GLFW_PRESS && right_click == GLFW_RELEASE && Window::mouse_status == IDLE) {
		Window::mouse_status = LEFT_HOLD;
		Window::lastPoint = Window::curPoint;
	}
	//Right click hold will save the position that the mouse was clicked and save it.
	else if (right_click == GLFW_PRESS && left_click == GLFW_RELEASE && Window::mouse_status == IDLE) {
		Window::mouse_status = RIGHT_HOLD;
		Window::lastPoint = Window::curPoint;
	}
	//If left click is held, then released, reset back to idle.
	else if (left_click == GLFW_RELEASE && Window::mouse_status == LEFT_HOLD) {
		Window::mouse_status = IDLE;
	}
	//If right click is held, then released, reset back to idle.
	else if (right_click == GLFW_RELEASE && Window::mouse_status == RIGHT_HOLD) {
		Window::mouse_status = IDLE;
	}
}

/* Handle mouse scroll input. */
void Window::cursor_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

/* Update the camera given e, d, and up vectors. We essentially rewrite the current camera. */
void Window::updateCamera(glm::vec3 e, glm::vec3 d, glm::vec3 up)
{
	Window::camera_pos = e;
	Window::camera_look_at = d;
	Window::camera_up = up;
	Window::V = glm::lookAt(e, d, up);
}
