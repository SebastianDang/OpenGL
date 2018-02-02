#include "stdafx.h"
#include "Geo_Object.h"

Geo_Object::Geo_Object()
{
	ResetToWorld();
}

Geo_Object::~Geo_Object()
{
	// The buffer objects are cleared in the base class. Delete anything else created here.
}

void Geo_Object::LoadCube()
{
	GLfloat vertices_array[] = 
	{
		// Front vertices
		-1.0f, -1.0f,  1.0f, // bottom left
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f, // top left

		// Back vertices
		-1.0f, -1.0f, -1.0f, // bottom left
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f // top left
	};

	GLuint indices_array[] = 
	{  // Note that we start from 0!
		// Front face
		0, 1, 2,
		2, 3, 0,
		// Top face
		1, 5, 6,
		6, 2, 1,
		// Back face
		7, 6, 5,
		5, 4, 7,
		// Bottom face
		4, 0, 3,
		3, 7, 4,
		// Left face
		4, 5, 1,
		1, 0, 4,
		// Right face
		3, 2, 6,
		6, 7, 3
	};

	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	// Data.
	for (int i = 0; i <= 21; i+=3)
	{
		vertices.push_back(glm::vec3(vertices_array[i], vertices_array[i + 1], vertices_array[i + 2]));
		normals.push_back(glm::vec3(0.0f));
		texCoords.push_back(glm::vec2(0.0f));
	}

	for (int i = 35; i >= 0; i--)
		indices.push_back(indices_array[i]);

	// Load the data from the cube we created.
	int success = LoadData(vertices, normals, texCoords, indices, 24);
	if (success != -1) LoadDataIntoBuffers();

	// Set some default material for now.
	Material mat;
	mat.SetAmbient(glm::vec3(0.24725f, 0.2245f, 0.0645f));
	mat.SetDiffuse(glm::vec3(0.34615f, 0.3143f, 0.0903f));
	mat.SetSpecular(glm::vec3(0.797357f, 0.723991f, 0.208006f));
	mat.SetShininess(83.2f);
	AddMaterial(mat);
}

void Geo_Object::LoadSphere()
{
}

void Geo_Object::Render(Shader * pShaderProgram)
{
	if (!IsInit() || !pShaderProgram) return;

	// Update any materials this has.
	for (int i = 0; i < (int)m_Materials.size(); i++)
	{
		m_Materials[i].UpdateShader(pShaderProgram);
	}

	// Update any variables that are with this object.
	pShaderProgram->Set("model", m_ToWorld);

	// Use this shader.
	pShaderProgram->Use();

	// Bind for rendering.
	glBindVertexArray(m_VAO);

	// Choose drawing method.
	int DataCount = (int)m_Data.size();
	int IndicesCount = (int)m_Indices.size();

	if (DataCount > 0 && IndicesCount <= 0)
		glDrawArrays(GL_TRIANGLES, 0, DataCount);

	else if (DataCount > 0 && IndicesCount > 0)
		glDrawElements(GL_TRIANGLES, (GLsizei)IndicesCount, GL_UNSIGNED_INT, 0);

	// Unbind.
	glBindVertexArray(0);
}
