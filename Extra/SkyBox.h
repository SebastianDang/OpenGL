#ifndef SkyBox_H
#define SkyBox_H

#include "Window.h"

class SkyBox
{
private:

	// Data
	std::vector<glm::vec3> m_Vertices; // v
	std::vector<unsigned int> m_Indices; // f
	std::vector<const GLchar*> m_Faces; // Inside face walls.

	// OpenGL buffers
	GLuint VAO, VBO, EBO;

	// Keep track of world in relation to the object.
	glm::mat4 toWorld;

	unsigned char* loadPPM(const char* filename, int& width, int& height);

	void setupCube();
	void setupFaces();
	void setupSkyBox();

	GLuint loadCubemap(std::vector<const GLchar*> faces);

public:
	SkyBox();
	~SkyBox();

	void draw(GLuint);
};

#endif

