#ifndef WINDOW_H
#define WINDOW_H

//OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Additional libraries
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <vector>

//External header files
#include "shader.h"

class Window
{
private:

public:
	//Window size properties
	static int width;
	static int height;

	//Mouse properties
	static int mouse_status;
	static glm::vec3 lastPoint;
	static glm::vec3 curPoint;
	
	//Camera properties
	static glm::vec3 camera_pos;
	static glm::vec3 camera_look_at;
	static glm::vec3 camera_up;

	//Matrix Coordinate Transformation properties
	static glm::mat4 P;
	static glm::mat4 V;

	//Frame Time calculation
	static float lastFrameTime;
	static float delta;

	//Window functions
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void frame_time_callback();
	static void display_callback(GLFWwindow*);
	static void drawScene();
	static void drawEnvironment(glm::vec4 clipPlane = glm::vec4(0.0f));

	//Keyboard and mouse callback functions
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static void cursor_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	//Camera update function
	static void camera_update_callback(glm::vec3 e, glm::vec3 d, glm::vec3 up);
};

#endif
