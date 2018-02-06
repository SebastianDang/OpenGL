#include "stdafx.h"
#include "Skybox.h"

Skybox::Skybox(std::vector<const char*> faces)
{
	ResetToWorld();

	// Store all the faces.
	for (const char *face : faces) m_Faces.push_back(face);

	// Load the cubemaps at this step.
	if ((int)m_Faces.size() > 0)
	{
		m_CubemapTexture = Object::LoadCubemap(m_Faces);
	}

	// Load the data from the cube we created.
	int success = LoadCube(100.0f);
	if (success == 0)
	{
		LoadDataIntoBuffers();
	}
}

Skybox::~Skybox()
{
}

void Skybox::Render(Shader * pShaderProgram)
{
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

