#pragma once

#include "Shader.h"

/// <summary>
/// Container: [X, Y, Z] [R, G, B] [S, T]
/// For each data point, we store the vertex, normal, and texture coordinates.
/// </summary>
struct S_Container
{
	// Position
	glm::vec3 m_Vertex = glm::vec3(0.0f);
	// Normal
	glm::vec3 m_Normal = glm::vec3(0.0f);
	// TexCoords
	glm::vec2 m_TexCoord = glm::vec2(0.0f);
};

/// <summary>
/// Base object class that includes loading from file, geometry, and generates buffers to draw using OpenGL.
/// Any class that inherits this, must implement the Render function.
/// </summary>
class Object
{
private:
	bool m_IsInit = false;

protected:
	// Data
	std::vector<S_Container> m_Data;
	std::vector<unsigned int> m_Indices; 
	
	// OpenGL buffers
	GLuint m_VAO = 0, m_VBO = 0, m_EBO = 0;

	// Keep track of world in relation to the object.
	glm::mat4 m_ToWorld = glm::mat4(1.0f);

public:
	Object();
	~Object();

	/// <summary>
	///  Load a ppm file from disk.
	/// </summary>
	/// <param name="filename">The location of the PPM file.  If the file is not found, an error message</param>
	/// <param name="width">This will be modified to contain the width of the loaded image, or 0 if file not found</param>
	/// <param name="height">This will be modified to contain the height of the loaded image, or 0 if file not found</param>
	/// <returns>RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured</returns>
	static unsigned char* LoadPPM(const char* filename, int& width, int& height);

	static GLuint LoadCubemap(std::vector<const char*> faces);

	/// <summary>
	/// Set to true when the buffers objects are generated and loaded. This means it's ready to 'render'.
	/// </summary>
	/// <returns>Returns the Init member variable</returns>
	bool IsInit() { return m_IsInit; }

	SETGET(glm::mat4, ToWorld);

	/// <summary>
	/// Resets the model/world matrix to the identity matrix.
	/// </summary>
	void ResetToWorld() { m_ToWorld = glm::mat4(1.0f); }

	/// <summary>
	/// Loads data from scratch, using defined vertices, normals, texCoords, indices. The count is how many to add.
	/// </summary>
	/// <param name="vertices">Vertex</param>
	/// <param name="normals">Normal</param>
	/// <param name="texCoords">Texture Coordinates</param>
	/// <param name="indices">Order of drawing, if we want to use glDrawElements, if not, then glDrawArrays will be used.</param>
	/// <param name="count">Number of elements to load into the object.</param>
	/// <returns></returns>
	int LoadData(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &texCoords, const std::vector<unsigned int> &indices, const int &count);

	/// <summary>
	/// Loads geometry data from file. 
	/// TODO: Implement this to open multiple obj type files, possibly convert the whole thing to draw meshes instead.
	/// Possibly use assimp.
	/// </summary>
	/// <param name="file"></param>
	/// <returns></returns>
	int LoadDataFromFile(const char *file);
	
	/// <summary>
	///  Load geometry data stored, and put it into the VAO, VBO, so that we can render it.
	/// </summary>
	void LoadDataIntoBuffers();

	/// <summary>
	/// Translate the object. 
	/// Stacks the current transform onto the current model/world matrix.
	/// </summary>
	/// <param name="translation">Displacement vector</param>
	void Translate(const glm::vec3 &value);
	
	/// <summary>
	/// Rotate the object around an arbitrary axis, in degrees.  
	/// Stacks the current transform onto the current model/world matrix.
	/// </summary>
	/// <param name="degree">Degree of rotation</param>
	/// <param name="axis">Axis of rotation</param>
	void Rotate(const float degree, const glm::vec3 &axis);

	/// <summary>
	/// Scale the object, either uniformly, or non-uniformly.
	/// Stacks the current transform onto the current model/world matrix.
	/// </summary>
	/// <param name="value">Vector to apply scaling to.</param>
	void Scale(const glm::vec3 &value);

	/// <summary>
	/// Sets the position to a fixed value.
	/// </summary>
	/// <param name="value"></param>
	void SetPosition(const glm::vec3 &value);
	
	/// <summary>
	/// Render that has to be implemented in any inherited class.
	/// </summary>
	/// <param name="pShaderProgram">Shader that's used for this object</param>
	virtual void Render(Shader *pShaderProgram) = 0;
};
