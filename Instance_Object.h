#pragma once
#include "Object.h"

class Instance_Object : public Object
{
private:

	Object *m_pObject = nullptr;
	Material *m_pMaterial = nullptr;

public:
	Instance_Object();
	Instance_Object(Object &obj);
	~Instance_Object();

	void SetObject(Object &obj);

	void SetMaterial(Material &mat);

	void Render(Shader *pShaderProgram);
};

