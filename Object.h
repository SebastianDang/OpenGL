#pragma once

#include "Shader.h"

/// <summary>
/// Texture id, with a string type.
/// </summary>
class Texture
{
private:
	GLuint m_Id;
	std::string m_Type;

public:
	SETGET(GLuint, Id);
	SETGET(std::string, Type);
};

/// Represent the material for the object.
/// TODO: Implement other types for unique material types.
/// </summary>
class Material
{
private:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;

public:
	SETGET(glm::vec3, Ambient);
	SETGET(glm::vec3, Diffuse);
	SETGET(glm::vec3, Specular);
	SETGET(float, Shininess)

	void UpdateShader(Shader *pShaderProgram);
};

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
	std::vector<Texture> m_Textures;
	std::vector<Material> m_Materials;

	// OpenGL buffers
	GLuint m_VAO, m_VBO, m_EBO;

	// Keep track of world in relation to the object.
	glm::mat4 m_ToWorld;

public:
	Object();
	~Object();

	/// <summary>
	/// Set to true when the buffers objects are generated and loaded. This means it's ready to 'render'.
	/// </summary>
	/// <returns>Returns the Init member variable</returns>
	bool IsInit() { return m_IsInit; }

	/// <summary>
	///  Load a ppm file from disk.
	/// </summary>
	/// <param name="filename">The location of the PPM file.  If the file is not found, an error message</param>
	/// <param name="width">This will be modified to contain the width of the loaded image, or 0 if file not found</param>
	/// <param name="height">This will be modified to contain the height of the loaded image, or 0 if file not found</param>
	/// <returns>RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured</returns>
	unsigned char* LoadPPM(const char* filename, int& width, int& height);

	/// <summary>
	/// Loads data from scratch, using defined vertices, normals, texCoords, indices. The count is how many to add.
	/// </summary>
	/// <param name="vertices">Vertex</param>
	/// <param name="normals">Normal</param>
	/// <param name="texCoords">Texture Coordinates</param>
	/// <param name="indices">Order of drawing, if we want to use glDrawElements, if not, then glDrawArrays will be used.</param>
	/// <param name="count">Number of elements to load into the object.</param>
	/// <returns></returns>
	int LoadData(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, std::vector<unsigned int> indices, int count);

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
	/// 
	/// </summary>
	/// <param name="texture"></param>
	void AddTexture(Texture texture);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="material"></param>
	void AddMaterial(Material material);

	/// <summary>
	/// Simple getter for the model/world matrix.
	/// </summary>
	/// <returns>The current model/world matrix for the object</returns>
	glm::mat4 GetToWorld() { return m_ToWorld; }
	
	/// <summary>
	/// Resets the model/world matrix to the identity matrix.
	/// </summary>
	void ResetToWorld() { m_ToWorld = glm::mat4(1.0f); }

	/// <summary>
	/// Translate the object. 
	/// Stacks the current transform onto the current model/world matrix.
	/// </summary>
	/// <param name="translation">Displacement vector</param>
	void Translate(glm::vec3 translation);
	
	/// <summary>
	/// Rotate the object around an arbitrary axis, in degrees.  
	/// Stacks the current transform onto the current model/world matrix.
	/// </summary>
	/// <param name="degree">Degree of rotation</param>
	/// <param name="axis">Axis of rotation</param>
	void Rotate(float degree, glm::vec3 axis);

	/// <summary>
	/// Scale the object, either uniformly, or non-uniformly.
	/// Stacks the current transform onto the current model/world matrix.
	/// </summary>
	/// <param name="value">Vector to apply scaling to.</param>
	void Scale(glm::vec3 value);
	
	/// <summary>
	/// Render that has to be implemented in any inherited class.
	/// </summary>
	/// <param name="pShaderProgram">Shader that's used for this object</param>
	virtual void Render(Shader *pShaderProgram) = 0;
};
