#pragma once

#ifndef LIGHT_H
#define LIGHT_H

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
	SETGET(bool, Enabled);
	SETGET(glm::vec3, Position);
	SETGET(glm::vec3, Ambient);
	SETGET(glm::vec3, Diffuse);
	SETGET(glm::vec3, Specular);

	virtual void UpdateShader(Shader *pShaderProgram) = 0;
};

class DirectionalLight : public Light
{
private:
	glm::vec3 m_Direction;

public:
	DirectionalLight() {}

	SETGET(glm::vec3, Direction);

	void UpdateShader(Shader *pShaderProgram);
};

class PointLight : public Light
{
private:
	float m_Quadratic;

public:
	PointLight() {}

	SETGET(float, Quadratic);

	void UpdateShader(Shader *pShaderProgram);
};

class SpotLight : public Light
{
private:
	glm::vec3 m_Direction;
	float m_Quadratic;
	float m_SpotCutoff;
	float m_SpotExponent;

public:
	SpotLight() {}

	SETGET(glm::vec3, Direction);
	SETGET(float, Quadratic);
	SETGET(float, SpotCutoff);
	SETGET(float, SpotExponent);

	void UpdateShader(Shader *pShaderProgram);
};

#endif