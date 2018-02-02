#pragma once
#include "Object.h"

/// <summary>
/// Load a simple obj file and use it as a renderable object.
/// </summary>
class Obj_Object : public Object
{
private:
	std::string m_Path;
	std::string m_File;

public:

	/// <summary>
	/// Constructor that loads from the filepath and sets path, file member variables.
	/// </summary>
	/// <param name="filepath">Relative filepath</param>
	Obj_Object(const char *filepath);

	/// <summary>
	/// Destructor that also calls the base class destructor.
	/// </summary>
	~Obj_Object();

	/// <summary>
	/// Pure virtual function that's inherited in this class.
	/// </summary>
	/// <param name="pShaderProgram">Shader to render the object</param>
	void Render(Shader *pShaderProgram);

	/// <summary>
	/// Sets the position in world space.
	/// </summary>
	/// <param name="pos">New position value</param>
	void SetPosition(glm::vec3 pos);
};

