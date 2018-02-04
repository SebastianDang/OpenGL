#include "stdafx.h"
#include "Skybox.h"

Skybox::Skybox(std::vector<const char*> faces)
{
	ResetToWorld();

	// Store all the faces.
	for (const char *face : faces) m_Faces.push_back(face);

	// Load the data from the cube we created.
	int success = LoadCube(100.0f);
	if (success == 0) LoadDataIntoBuffers();
}

Skybox::~Skybox()
{
}

void Skybox::LoadDataIntoBuffers()
{
	// Make sure we actually have data first. This checks if data was loaded yet.
	if ((int)m_Data.size() < 0) return;

	// Create buffers/arrays.
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(m_VAO); //Bind vertex array object.

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO); //Bind Container buffer.
	glBufferData(GL_ARRAY_BUFFER, m_Data.size() * sizeof(S_Container), &m_Data[0], GL_STATIC_DRAW); // Set vertex buffer to the Container.

	// Vertex Positions.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(S_Container), (GLvoid*)offsetof(S_Container, m_Vertex));

	// Vertex Normals.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(S_Container), (GLvoid*)offsetof(S_Container, m_Normal));

	// Vertex Texture Coords.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(S_Container), (GLvoid*)offsetof(S_Container, m_TexCoord));

	// If there are indices, we can set them here, otherwise we don't use them.
	if ((int)m_Indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); //Bind indices buffer.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(int), &m_Indices[0], GL_STATIC_DRAW);
	}

	// Load the cubemaps at this step.
	if ((int)m_Faces.size() > 0)
	{
		m_CubemapTexture = Object::LoadCubemap(m_Faces);
	}

	// Unbind.
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind.
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO.
}

void Skybox::Render(Shader * pShaderProgram)
{
	if (!pShaderProgram) return;

	// Draw skybox.
	glDepthMask(GL_FALSE);// Remember to turn depth writing off.

	// Skybox cube.
	glBindVertexArray(m_VAO); // Bind the vertex.
	glActiveTexture(GL_TEXTURE0);// Enable the texture.
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);// Bind the cubemapTexture.
	glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);// Unbind vertex.
	glDepthMask(GL_TRUE);// Turn depth writing back on.

	// Culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

