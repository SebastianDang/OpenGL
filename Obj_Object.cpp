#include "stdafx.h"
#include "Obj_Object.h"

Obj_Object::Obj_Object(const char * filepath)
	: Object() // Add a call to the superclass constructor in case we need it.
{
	// Break up the filepath, so we can store it for later.
	std::string fullPath = std::string(filepath);
	int slash = (int)fullPath.find_last_of("\\/");
	if (slash != std::string::npos)
	{
		m_Path = fullPath.substr(0, slash);
		m_File = fullPath.substr(slash + 1);
	}

	// Load the data from the filepath specified
	int success = LoadDataFromFile(filepath);
	if (success != -1) LoadDataIntoBuffers();

	// Set some default material for now.
	Material mat;
	mat.SetAmbient(glm::vec3(0.24725f, 0.2245f, 0.0645f));
	mat.SetDiffuse(glm::vec3(0.34615f, 0.3143f, 0.0903f));
	mat.SetSpecular(glm::vec3(0.797357f, 0.723991f, 0.208006f));
	mat.SetShininess(83.2f);
	AddMaterial(mat);
}

Obj_Object::~Obj_Object()
{
	// The buffer objects are cleared in the base class. Delete anything else created here.
}

void Obj_Object::Render(Shader *pShaderProgram)
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

void Obj_Object::SetPosition(glm::vec3 pos)
{
	m_ToWorld[3] = glm::vec4(pos, 1.0f);
}
