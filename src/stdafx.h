#pragma once

// Include libraries
#include <algorithm>
#include <cstring>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

// OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM libraries
#pragma warning(disable:4127)
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(default:4127) 

// Constants
#define ASSETS_FOLDER "./assets"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define MOUSE_SENSITIVITY 0.01f 

// Macro to generate quick set/get per class. Requires that the variable is defined as 'm_variable'.
#define SETGET(type,var) \
	void Set##var(type val) { m_##var = val; } \
	type Get##var(void) const { return m_##var; }

