#include "stdafx.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Shader.h"

using namespace std;

// Set the title of the program. This will remain static.
const char* window_title = "OpenGL";

// Window properties.
float Window::FieldOfView = 45.0f;
float Window::NearPlane = 0.1f;
float Window::FarPlane = 1000.0f;
int Window::Width; // Width of the window (This will be set in window_resize callback).
int Window::Height; // Height of the window (This will be set in window_resize callback).

// Mouse properties.
int Window::Mouse_Status = E_MOUSE::IDLE; // Define the mouse status for any clicks.
glm::vec3 Window::LastPoint; // Last point clicked.
glm::vec3 Window::CurrPoint; // Current point clicked.

// Frame Time calculation.
float Window::LastFrameTime; // Last frame time recorded.
float Window::DeltaFrameTime; // The time since the last frame time, from the current frame time.

// Matrix Coordinate Transformation properties.
glm::mat4 Window::P; // Perspective.
glm::mat4 Window::V; // View.

// Reference to the main ResourceManager, to get all diferent objects/things in our environment.
ResourceManager *pManager = nullptr;

// Create window is called to initialize the window.
GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context this one
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	// Set the initial frame time.
	Window::LastFrameTime = (float)glfwGetTime();

	// Mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	return window;
}

// Initialize
void Window::Start()
{
	pManager = new ResourceManager();
	pManager->Load(); // Load from file.
	if (pManager) { if (pManager->GetCurrentCamera()) Window::V = pManager->GetCurrentCamera()->GetMatrix(); }
}

// Deconstructor, deletes all initialized objects for a proper cleanup.
void Window::Stop()
{
	if (pManager) delete(pManager);
}

// Window callback function on a resize. Resets the perspective for proper coordinate transformation.
void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	// Update width and height
	Window::Width = width;
	Window::Height = height;

	// Set the viewport size
	glViewport(0, 0, width, height);

	if (height > 0) 
		Window::P = glm::perspective(Window::FieldOfView, (float)width / (float)height, Window::NearPlane, Window::FarPlane);

	// Set the LastPoint to be the center.
	Window::LastPoint = glm::vec3(float(width / 2), float(height / 2), 0.0f);
}

/* Idle callback function. This will call itself repeatedly as long as the program is running. */
void Window::idle_callback()
{
	// Calculate the frame time.
	Window::frame_time_callback();
}

/* Perform updates to the frame time for any animations. */
void Window::frame_time_callback()
{
	// Calculate the frame time.
	float currentFrameTime = (float)glfwGetTime();//Get the current time.
	Window::DeltaFrameTime = (currentFrameTime - Window::LastFrameTime);//Calculate the change from this frame time, to the old frame time.
	Window::LastFrameTime = currentFrameTime;//Record the (new per calculation) old frame time.
}

/* Display callback function. Whenever contents need to be redisplayed, this will be called. We render, or redraw any part of the scene here. */
void Window::display_callback(GLFWwindow* window)
{
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Clear color and bits.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (pManager)
	{
		// For each light
		for (int l = 0; l < pManager->GetNumLights(); l++)
		{
			Light *pLight = pManager->GetLight(l);
			if (pLight)
			{
				Shader *pShader = pManager->GetShader("object");
				if (pShader)
				{
					pShader->Use();
					pShader->Set("view", Window::V);
					pShader->Set("projection", Window::P);
					pLight->UpdateShader(pShader);
				}
			}
		}

		// For each object
		for (int i = 0; i < pManager->GetNumObjects(); i++)
		{
			// Get the object out.
			Object *pObject = pManager->GetObject(i);

			// Special case for skybox.
			Skybox *pSky = dynamic_cast<Skybox*>(pObject);
			if (pSky)
			{
				Shader *pSkyShader = pManager->GetShader("skybox");
				if (pSkyShader)
				{
					pSkyShader->Use();
					pSkyShader->Set("view", glm::mat4(glm::mat3(Window::V)));
					pSkyShader->Set("projection", Window::P);
					pObject->Render(pSkyShader);
				}
			}
			else
			{
				Shader *pShader = pManager->GetShader("object");
				if (pShader)
				{
					pShader->Use();
					pShader->Set("view", Window::V);
					pShader->Set("projection", Window::P);
					pObject->Render(pShader);
				}
			}
		}
	}

	// Swap buffers
	glfwSwapBuffers(window);
	glFinish();
}

/* Handle Key input. */
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Define shift keys for capital letters.
	int Lshift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	int Rshift = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);

	// Define movement keys.
	int wKey = glfwGetKey(window, GLFW_KEY_W);
	int aKey = glfwGetKey(window, GLFW_KEY_A);
	int sKey = glfwGetKey(window, GLFW_KEY_S);
	int dKey = glfwGetKey(window, GLFW_KEY_D);

	if (wKey == GLFW_PRESS)
	{
	}
	if (aKey == GLFW_PRESS)
	{
	}
	if (sKey == GLFW_PRESS)
	{
	}
	if (dKey == GLFW_PRESS)
	{
	}

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
		if (key == GLFW_KEY_T) {
		}
	}
}

/* Handle mouse movement input. */
void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Update current mouse position.
	glfwGetCursorPos(window, &xpos, &ypos);
	
	// Get current mouse position.
	Window::CurrPoint = glm::vec3(xpos, ypos, 0.0f);

	//  On left drag, we perform rotations. Relative to the object.
	if (Window::Mouse_Status == E_MOUSE::LEFT_HOLD)
	{
		if (pManager && pManager->GetCurrentCamera())
		{
			ThirdPersonCamera *pCamera = dynamic_cast<ThirdPersonCamera*>(pManager->GetCurrentCamera());
			if (!pCamera) return;
			pCamera->RotateAroundPoint(pCamera->GetFollowPoint(), Window::LastPoint, Window::CurrPoint);
			Window::V = pCamera->GetMatrix();
		}
	}

	// On right drag, we perform translations. Relative to the object.
	if (Window::Mouse_Status == E_MOUSE::RIGHT_HOLD)
	{
		if (pManager && pManager->GetCurrentCamera())
		{
			FirstPersonCamera *pCamera = dynamic_cast<FirstPersonCamera*>(pManager->GetCurrentCamera());
			if (!pCamera) return;
			pCamera->Move(Window::LastPoint, Window::CurrPoint);
			Window::V = pCamera->GetMatrix();
		}
	}
}

/* Handle mouse button input. This handles the status if left button or right button was clicked and held. */
void Window::cursor_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Define left and right clicks.
	int left_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int right_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	// Left click hold will save the position that the mouse was clicked and save it.
	if (left_click == GLFW_PRESS && right_click == GLFW_RELEASE && Window::Mouse_Status == E_MOUSE::IDLE) {
		Window::Mouse_Status = E_MOUSE::LEFT_HOLD;
		Window::LastPoint = Window::CurrPoint;
	}
	// Right click hold will save the position that the mouse was clicked and save it.
	else if (right_click == GLFW_PRESS && left_click == GLFW_RELEASE && Window::Mouse_Status == E_MOUSE::IDLE) {
		Window::Mouse_Status = E_MOUSE::RIGHT_HOLD;
		Window::LastPoint = Window::CurrPoint;
	}
	// If left click is held, then released, reset back to idle.
	else if (left_click == GLFW_RELEASE && Window::Mouse_Status == E_MOUSE::LEFT_HOLD) {
		Window::Mouse_Status = E_MOUSE::IDLE;
	}
	// If right click is held, then released, reset back to idle.
	else if (right_click == GLFW_RELEASE && Window::Mouse_Status == E_MOUSE::RIGHT_HOLD) {
		Window::Mouse_Status = E_MOUSE::IDLE;
	}
}

/* Handle mouse scroll input. */
void Window::cursor_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (pManager && pManager->GetCurrentCamera())
	{
		ThirdPersonCamera *pCamera = dynamic_cast<ThirdPersonCamera*>(pManager->GetCurrentCamera());
		if (!pCamera) return;
		pCamera->Zoom(yoffset);
		Window::V = pCamera->GetMatrix();
	}
}
