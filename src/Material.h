#pragma once

#include "Shader.h"

/// Represent the material for the object.
/// TODO: Implement other types for unique material types.
/// </summary>
class Material
{
private:
	glm::vec3 m_Ambient = glm::vec3(0.0f);
	glm::vec3 m_Diffuse = glm::vec3(0.0f);
	glm::vec3 m_Specular = glm::vec3(0.0f);
	float m_Shininess = 0.0f;

public:
	Material();
	~Material();

	SETGET(glm::vec3, Ambient);
	SETGET(glm::vec3, Diffuse);
	SETGET(glm::vec3, Specular);
	SETGET(float, Shininess)

	void UpdateShader(Shader *pShaderProgram);
};

