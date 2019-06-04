#pragma once

#include "Land.h"
#include "Object.h"

class Terrain : public Land, public Object
{
private:

	float m_Size[2] = { 0.0f, 0.0f };
	int m_VertexCount = 0;
	int m_DrawingMode = 1;
	glm::vec3 m_Translation = glm::vec3(0.0f);

public:
	Terrain();
	~Terrain();

	int GetNumVertices() { return m_VertexCount; }

	int LoadDataFromSimpleGrid(float x, float z, float height, int vertexCount);

	glm::vec3 GetVertex(int index);
	void SetVertex(glm::vec3 &value, int index);

	glm::vec3 GetNormal(int index);
	void SetNormal(glm::vec3 &value, int index);

	void ResetGrid(float height);

	// Smoothing algorithms.
	void SmoothUsingDiamondSquare(int x1, int x2, int y1, int y2, int level, float range, float max);

	void Render(Shader * pShaderProgram);

};