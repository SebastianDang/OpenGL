#pragma once
#include "Object.h"

class Skybox : public Object
{
private:
	std::vector<const char *> m_Faces;
	GLuint m_CubemapTexture;

public:
	Skybox();
	~Skybox();

	GLuint LoadCubemap(std::vector<const GLchar*> faces);

	/// <summary>
	/// Render the geometry object.
	/// </summary>
	/// <param name="pShaderProgram">Shader to render the object</param>
	void Render(Shader *pShaderProgram);
};

