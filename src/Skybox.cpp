#include "stdafx.h"
#include "Skybox.h"

#define SKYBOX_CONSTANT_SIZE 100.0f

Skybox::Skybox(std::vector<const char*> faces)
	: Geo_Object()
{
	// Load the data from the faces we passed in.
	int success = LoadDataFromFaces(faces);
	if (success != -1) LoadDataIntoBuffers();
}

Skybox::~Skybox()
{
}

int Skybox::LoadDataFromFaces(std::vector<const char*> faces)
{
	// Store all the faces.
	for (const char *face : faces) m_Faces.push_back(face);

	// Load the cubemaps at this step.
	if ((int)m_Faces.size() > 0)
	{
		m_CubemapTexture = Object::LoadCubemap(m_Faces);
	}

	// Load the data from the cube we created.
	return LoadDataForCube(SKYBOX_CONSTANT_SIZE);
}

void Skybox::Render(Shader * pShaderProgram)
{
	// If the buffers aren't loaded or the shader program isn't initialized, we don't render.
	if (!IsInit() || !pShaderProgram) return;

	// Draw skybox.
	glDepthMask(GL_FALSE); // Remember to turn depth writing off.

	// Skybox cube.
	glBindVertexArray(m_VAO); // Bind the vertex.
	glActiveTexture(GL_TEXTURE0); // Enable the texture.
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture); // Bind the cubemapTexture.
	glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // Unbind vertex.
	glDepthMask(GL_TRUE); // Turn depth writing back on.

	// Culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

