#include "stdafx.h"
#include "Terrain.h"
#include <time.h>
#include <math.h>

#define DRAW_SHADED 0
#define DRAW_WIREFRAME 1

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

int Terrain::LoadDataFromSimpleGrid(float x, float z, float height, int vertexCount)
{
	// Set properties.
	m_Size[0] = x;
	m_Size[1] = z;
	m_VertexCount = vertexCount;
	Translate(glm::vec3(-m_Size[0] / 2.0f, 0.0f, -m_Size[1] / 2.0f)); // Translate it to the center.

	// Before we begin, clear data.
	ResetData();

	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	// Generate vertices, normals, and texCoords for a simple flat grid map. vertex = (j, i).
	for (int i = 0; i < vertexCount; i++)
	{
		for (int j = 0; j < vertexCount; j++)
		{
			// Setup the vertices.
			float vertex_x = (float)j / ((float)vertexCount - 1) * x;
			float vertex_y = height;
			float vertex_z = (float)i / ((float)vertexCount - 1) * z;

			// Setup the normals.
			float normal_x = 0;
			float normal_y = 1.0f;
			float normal_z = 0;

			// Setup the texcoords.
			float texCoord_x = (float)j / ((float)vertexCount - 1);
			float texCoord_y = (float)i / ((float)vertexCount - 1);

			// Push back to vectors.
			vertices.push_back(glm::vec3(vertex_x, vertex_y, vertex_z));
			normals.push_back(glm::vec3(normal_x, normal_y, normal_z));
			texCoords.push_back(glm::vec2(texCoord_x, texCoord_y));
		}
	}

	// Setup the indices to draw based on indice points.
	for (int gz = 0; gz < vertexCount - 1; gz++)
	{
		for (int gx = 0; gx < vertexCount - 1; gx++)
		{
			int topLeft = (gz*vertexCount) + gx;//0//1
			int topRight = topLeft + 1;//1//2
			int bottomLeft = ((gz + 1)*vertexCount) + gx;//128//129
			int bottomRight = bottomLeft + 1;//129//130

			// Push back to indices.
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	// Load the data from the obj we loaded.
	return LoadData(vertices, normals, texCoords, indices, (int)vertices.size());
}

glm::vec3 Terrain::GetVertex(int index)
{
	if (index < 0 || index >= (int)m_Data.size()) return glm::vec3();
	return m_Data[index].m_Vertex;
}

void Terrain::SetVertex(glm::vec3 & value, int index)
{
	if (index < 0 || index >= (int)m_Data.size()) return;
	m_Data[index].m_Vertex = value;
}

glm::vec3 Terrain::GetNormal(int index)
{
	if (index < 0 || index >= (int)m_Data.size()) return glm::vec3();
	return m_Data[index].m_Normal;
}

void Terrain::SetNormal(glm::vec3 & value, int index)
{
	if (index < 0 || index >= (int)m_Data.size()) return;
	m_Data[index].m_Normal = value;
}

void Terrain::ResetGrid(float height)
{
	for (int i = 0; i < (int)m_Data.size(); i++)
	{
		m_Data[i].m_Vertex.y = height;
	}
}

void Terrain::SmoothUsingDiamondSquare(int x1, int x2, int y1, int y2, int level, float range, float max)
{
	// Check base case to stop recursion.
	if (level < 1)
		return;

	// Start random number generation.
	srand((unsigned int)time(NULL));

	// Diamond Algorithm
	for (int i = x1 + level; i < x2; i += level)
	{
		for (int j = y1 + level; j < y2; j += level)
		{
			// Get the 4 main vertices.
			glm::vec3 vec_a = GetVertex((j - level)*m_VertexCount + (i - level));
			glm::vec3 vec_b = GetVertex((j - level)*m_VertexCount + i);
			glm::vec3 vec_c = GetVertex((j)*m_VertexCount + (i - level));
			glm::vec3 vec_d = GetVertex((j)*m_VertexCount + i);

			// Get the middle vertex.
			glm::vec3 vec_e = GetVertex((j - level / 2)*m_VertexCount + (i - level / 2));

			// Get the 4 heights.
			float height_a = vec_a.y;
			float height_b = vec_b.y;
			float height_c = vec_c.y;
			float height_d = vec_d.y;

			// Calculate the average height in the middle and set it to E.
			float height_e = (float)(height_a + height_b + height_c + height_d) / 4;
			height_e += fmod(((float)(rand()) / 1000), max)*range;
			vec_e.y = height_e;

			// Update data.
			SetVertex(vec_e, (j - level / 2)*m_VertexCount + (i - level / 2));
		}
	}

	// Square algorithm
	for (int i = x1 + 2 * level; i < x2; i += level)
	{
		for (int j = y1 + 2 * level; j < y2; j += level)
		{
			// Get the 4 main vertices.
			glm::vec3 vec_a = GetVertex((j - level)*m_VertexCount + (i - level));
			glm::vec3 vec_b = GetVertex((j - level)*m_VertexCount + i);
			glm::vec3 vec_c = GetVertex((j)*m_VertexCount + (i - level));
			//glm::vec3 vec_d = GetVertex((j)*m_VertexCount + i);

			// Get the middle vertex.
			glm::vec3 vec_e = GetVertex((j - level / 2)*m_VertexCount + (i - level / 2));

			// Get the 5 heights.
			float height_a = vec_a.y;
			float height_b = vec_b.y;
			float height_c = vec_c.y;
			//float height_d = vec_d.y;
			float height_e = vec_e.y;

			// Instead of doing all 4, since they overlap we do 2.
			// Calculate the average height and set it to F. 
			glm::vec3 vec_f = GetVertex((j - level / 2)*m_VertexCount + (i - level));
			float height_f = (float)(height_a + height_c + height_e + GetVertex((j - level / 2)*m_VertexCount + (i - 3 * level / 2)).y) / 3;
			height_f += fmod(((float)(rand()) / 1000), max)*range;
			vec_f.y = height_f;

			SetVertex(vec_f, (j - level / 2)*m_VertexCount + (i - level));

			// Calculate the average height and set it to G.
			glm::vec3 vec_g = GetVertex((j - level)*m_VertexCount + (i - level / 2));
			float height_g = (float)(height_a + height_b + height_e + GetVertex((j - 3 * level / 2)*m_VertexCount + (i - level / 2)).y) / 3;
			height_g += fmod(((float)(rand()) / 1000), max)*range;
			vec_g.y = height_g;

			SetVertex(vec_g, (j - level)*m_VertexCount + (i - level / 2));
		}
	}

	// Begin Recursion.
	SmoothUsingDiamondSquare(x1, x2, y1, y2, level / 2, range / 2.0f, max);
}

void Terrain::Render(Shader *pShaderProgram)
{
	// If the buffers aren't loaded or the shader program isn't initialized, we don't render.
	if (!IsInit() || !pShaderProgram) return;

	// Set draw_mode to view wireframe version or filled version.
	if (m_DrawingMode == DRAW_SHADED)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (m_DrawingMode == DRAW_WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

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

	// Set it back to fill.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}