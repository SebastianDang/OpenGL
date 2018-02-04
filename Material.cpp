#include "stdafx.h"
#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::UpdateShader(Shader * pShaderProgram)
{
	if (!pShaderProgram) return;
	pShaderProgram->Set("material.ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
	pShaderProgram->Set("material.diffuse", m_Diffuse.x, m_Diffuse.y, m_Diffuse.z);
	pShaderProgram->Set("material.specular", m_Specular.x, m_Specular.y, m_Specular.z);
	pShaderProgram->Set("material.shininess", m_Shininess);
	pShaderProgram->Set("reflect_intensity", m_Shininess / 100.0f);
}