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
	Obj_Object(const char* filepath);

	/// <summary>
	/// Destructor that also calls the base class destructor.
	/// </summary>
	~Obj_Object();

	// Setter/Getters for private variables.
	SETGET(std::string, Path);
	SETGET(std::string, File);

	/// <summary>
	/// Loads geometry data from file. 
	/// TODO: Implement this to open multiple obj type files, possibly convert the whole thing to draw meshes instead.
	/// Possibly use assimp.
	/// </summary>
	/// <param name="filepath"></param>
	/// <returns></returns>
	int LoadDataFromFile(const char* filepath);

	/// <summary>
	/// Render the obj object.
	/// </summary>
	/// <param name="pShaderProgram">Shader to render the object</param>
	void Render(Shader* pShaderProgram);
};

