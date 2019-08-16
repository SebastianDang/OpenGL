#include "stdafx.h"
#include "Instance_Object.h"


Instance_Object::Instance_Object()
	: Object()
{
}

Instance_Object::Instance_Object(Object& obj)
	: Object()
{
	SetObject(obj);
}

Instance_Object::~Instance_Object()
{
}

void Instance_Object::SetObject(Object& obj)
{
	m_pObject = &obj;
}

void Instance_Object::SetMaterial(Material& mat)
{
	m_pMaterial = &mat;
}

void Instance_Object::Render(Shader* pShaderProgram)
{
	if (m_pMaterial)
		m_pMaterial->UpdateShader(pShaderProgram);

	if (m_pObject)
	{
		m_pObject->SetToWorld(m_ToWorld);
		m_pObject->Render(pShaderProgram);
	}
}
