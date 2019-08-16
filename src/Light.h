#pragma once
#include "Shader.h"

class Light
{
protected:

	bool m_Enabled;
	glm::vec3 m_Position;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

public:

	Light()
		:
		m_Enabled(false),
		m_Position(0.0f),
		m_Ambient(0.0f),
		m_Diffuse(0.0f),
		m_Specular(0.0f)
	{}

	SETGET(bool, Enabled);
	SETGET(glm::vec3, Position);
	SETGET(glm::vec3, Ambient);
	SETGET(glm::vec3, Diffuse);
	SETGET(glm::vec3, Specular);

	virtual void UpdateShader(Shader* pShaderProgram) = 0;
};

class DirectionalLight : public Light
{
private:

	glm::vec3 m_Direction;

public:

	DirectionalLight()
		:
		Light(),
		m_Direction(0.0f)
	{}

	SETGET(glm::vec3, Direction);

	void UpdateShader(Shader* pShaderProgram);
};

class PointLight : public Light
{
private:

	float m_Quadratic;

public:

	PointLight()
		:
		Light(),
		m_Quadratic(0.0f)
	{}

	SETGET(float, Quadratic);

	void UpdateShader(Shader* pShaderProgram);
};

class SpotLight : public Light
{
private:

	glm::vec3 m_Direction;
	float m_Quadratic;
	float m_SpotCutoff;
	float m_SpotExponent;

public:

	SpotLight()
		:
		Light(),
		m_Direction(0.0f),
		m_Quadratic(0.0f),
		m_SpotCutoff(0.0f),
		m_SpotExponent(0.0f)
	{}

	SETGET(glm::vec3, Direction);
	SETGET(float, Quadratic);
	SETGET(float, SpotCutoff);
	SETGET(float, SpotExponent);

	void UpdateShader(Shader* pShaderProgram);
};
