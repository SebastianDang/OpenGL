#pragma once
#include "Geo_Object.h"

class Skybox : public Geo_Object
{
private:

	std::vector<const char*> m_Faces;
	GLuint m_CubemapTexture;

public:

	Skybox(std::vector<const char*> faces);
	~Skybox();

	int LoadDataFromFaces(std::vector<const char*> faces);

	// Setter/Getters for private variables.
	SETGET(GLuint, CubemapTexture);

	/// <summary>
	/// Render the geometry object.
	/// </summary>
	/// <param name="pShaderProgram">Shader to render the object</param>
	void Render(Shader* pShaderProgram);
};

