#pragma once
#include "Object.h"
#include "Material.h"

/// <summary>
/// 
/// </summary>
class Instance_Object : public Object
{
private:

	Object *m_pObject = nullptr;
	Material *m_pMaterial = nullptr;

public:

	Instance_Object();
	~Instance_Object();

	/// <summary>
	/// Takes an object and sets it up to render as an instance.
	/// </summary>
	/// <param name="obj">Loaded object, used somewhere or by something else.</param>
	Instance_Object(Object &obj);

	/// <summary>
	/// Sets the current object pointer.
	/// </summary>
	/// <param name="obj"></param>
	void SetObject(Object &obj);

	/// <summary>
	/// Sets the material to use for the shader.
	/// </summary>
	/// <param name="mat"></param>
	void SetMaterial(Material &mat);

	/// <summary>
	/// Calls render on the object.
	/// </summary>
	/// <param name="pShaderProgram"></param>
	void Render(Shader *pShaderProgram);
};

