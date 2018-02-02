#include "stdafx.h"
#include "Light.h"

void DirectionalLight::UpdateShader(Shader *pShaderProgram)
{
	pShaderProgram->Set("dirLight.on", m_Enabled);
	pShaderProgram->Set("dirLight.on", m_Enabled);
	pShaderProgram->Set("dirLight.direction", m_Direction.x, m_Direction.y, m_Direction.z);
	pShaderProgram->Set("dirLight.ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
	pShaderProgram->Set("dirLight.diffuse", m_Diffuse.x, m_Diffuse.y, m_Diffuse.z);
	pShaderProgram->Set("dirLight.specular", m_Specular.x, m_Specular.y, m_Specular.z);
}

void PointLight::UpdateShader(Shader *pShaderProgram)
{
	pShaderProgram->Set("pointLight.on", m_Enabled);
	pShaderProgram->Set("pointLight.position", m_Position.x, m_Position.y, m_Position.z);
	pShaderProgram->Set("pointLight.ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
	pShaderProgram->Set("pointLight.diffuse", m_Diffuse.x, m_Diffuse.y, m_Diffuse.z);
	pShaderProgram->Set("pointLight.specular", m_Specular.x, m_Specular.y, m_Specular.z);
	pShaderProgram->Set("pointLight.quadratic", m_Quadratic);
}

void SpotLight::UpdateShader(Shader * pShaderProgram)
{
	pShaderProgram->Set("spotLight.on", m_Enabled);
	pShaderProgram->Set("spotLight.position", m_Position.x, m_Position.y, m_Position.z);
	pShaderProgram->Set("spotLight.ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
	pShaderProgram->Set("spotLight.diffuse", m_Diffuse.x, m_Diffuse.y, m_Diffuse.z);
	pShaderProgram->Set("spotLight.specular", m_Specular.x, m_Specular.y, m_Specular.z);
	pShaderProgram->Set("spotLight.quadratic", m_Quadratic);
	pShaderProgram->Set("spotLight.direction", m_Direction.x, m_Direction.y, m_Direction.z);
	pShaderProgram->Set("spotLight.spotCutoff", m_SpotCutoff);
	pShaderProgram->Set("spotLight.spotExponent", m_SpotExponent);
}
