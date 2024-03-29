#include "stdafx.h"
#include "Geo_Object.h"

Geo_Object::Geo_Object()
	: Object()
{
}

Geo_Object::Geo_Object(float size)
{
	// Load the data for a cube.
	int success = LoadDataForCube(size);
	if (success != -1) LoadDataIntoBuffers();
}

Geo_Object::Geo_Object(float radius, int slices, int stacks)
{
	// Load the data for a sphere.
	int success = LoadDataForSphere(radius, slices, stacks);
	if (success != -1) LoadDataIntoBuffers();
}

Geo_Object::~Geo_Object()
{
	// The buffer objects are cleared in the base class. Delete anything else created here.
}

int Geo_Object::LoadDataForCube(float size)
{
	GLfloat vertices_array[] =
	{
		// Front vertices
		-size, -size,  size, // bottom left
		size, -size,  size,
		size,  size,  size,
		-size,  size,  size, // top left

		// Back vertices
		-size, -size, -size, // bottom left
		size, -size, -size,
		size,  size, -size,
		-size,  size, -size // top left
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
	for (int i = 0; i <= 21; i += 3)
	{
		vertices.push_back(glm::vec3(vertices_array[i], vertices_array[i + 1], vertices_array[i + 2]));
		normals.push_back(glm::vec3(0.0f));
		texCoords.push_back(glm::vec2(0.0f));
	}

	for (int i = 35; i >= 0; i--)
		indices.push_back(indices_array[i]);

	// Load the data from the cube we created.
	return LoadData(vertices, normals, texCoords, indices, 24);
}

int Geo_Object::LoadDataForSphere(float radius, int slices, int stacks)
{
	float fslices = (float)slices;
	float fstacks = (float)stacks;

	std::vector <glm::vec3> vertices, normals;

	for (int i = 0; i < slices; i++)
	{
		for (int j = 0; j < stacks; j++)
		{
			// Top left
			vertices.push_back(glm::vec3(
				radius * -cosf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices),
				radius * -cosf(glm::pi<float>() * (j + 1.0f) / fslices),
				radius * sinf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cosf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices),
				-cosf(glm::pi<float>() * (j + 1.0f) / fslices),
				sinf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices))));

			// Top right
			vertices.push_back(glm::vec3(
				radius * -cosf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices),
				radius * -cosf(glm::pi<float>() * (j + 1.0f) / fslices),
				radius * sinf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cosf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices),
				-cosf(glm::pi<float>() * (j + 1.0f) / fslices),
				sinf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices))));

			// Bottom right
			vertices.push_back(glm::vec3(
				radius * -cosf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * j / fslices),
				radius * -cosf(glm::pi<float>() * j / fslices),
				radius * sinf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * j / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cosf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * j / fslices),
				-cosf(glm::pi<float>() * j / fslices),
				sinf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * j / fslices))));

			// Need to repeat 2 of the vertices since we can only draw triangles. Eliminates the confusion
			// of array indices.
			// Top left
			vertices.push_back(glm::vec3(
				radius * -cosf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices),
				radius * -cosf(glm::pi<float>() * (j + 1.0f) / fslices),
				radius * sinf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cosf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices),
				-cosf(glm::pi<float>() * (j + 1.0f) / fslices),
				sinf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * (j + 1.0f) / fslices))));

			// Bottom right
			vertices.push_back(glm::vec3(
				radius * -cosf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * j / fslices),
				radius * -cosf(glm::pi<float>() * j / fslices),
				radius * sinf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * j / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cosf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * j / fslices),
				-cosf(glm::pi<float>() * j / fslices),
				sinf(2.0f * glm::pi<float>() * (i + 1.0f) / fstacks) * sinf(glm::pi<float>() * j / fslices))));

			// Bottom left
			vertices.push_back(glm::vec3(
				radius * -cosf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * j / fslices),
				radius * -cosf(glm::pi<float>() * j / fslices),
				radius * sinf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * j / fslices)));
			normals.push_back(glm::normalize(glm::vec3(
				-cosf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * j / fslices),
				-cosf(glm::pi<float>() * j / fslices),
				sinf(2.0f * glm::pi<float>() * i / fstacks) * sinf(glm::pi<float>() * j / fslices))));
		}
	}

	// Load the data from the sphere we created.
	return LoadData(vertices, normals, std::vector<glm::vec2>(), std::vector<unsigned int>(), (int)vertices.size());
}

void Geo_Object::Render(Shader* pShaderProgram)
{
	// If the buffers aren't loaded or the shader program isn't initialized, we don't render.
	if (!IsInit() || !pShaderProgram) return;

	// Update any variables that are with this object.
	pShaderProgram->Set("model", m_ToWorld);

	// Bind for rendering.
	glBindVertexArray(m_VAO);

	// Choose drawing method.
	int DataCount = (int)m_Data.size();
	int IndicesCount = (int)m_Indices.size();

	// Draw in the order data is defined. This is faster if data is chosen carefully.
	if (DataCount > 0 && IndicesCount <= 0)
		glDrawArrays(GL_TRIANGLES, 0, DataCount);

	// Draw in the order indices are defined.
	else if (DataCount > 0 && IndicesCount > 0)
		glDrawElements(GL_TRIANGLES, (GLsizei)IndicesCount, GL_UNSIGNED_INT, 0);

	// Unbind.
	glBindVertexArray(0);
}
