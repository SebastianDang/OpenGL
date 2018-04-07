#pragma once
#include "Object.h"

enum E_GeometryType
{
	Cube = 0,
	Sphere = 1
};

/// <summary>
/// Class to create simple shapes.
/// </summary>
class Geo_Object : public Object
{
private:
	int m_Type = -1;

public:
	Geo_Object();
	Geo_Object(float size);
	Geo_Object(float radius, int slices, int stacks);
	~Geo_Object();

	/// <summary>
	/// Create and load a cube, either into data.
	/// </summary>
	int LoadDataForCube(float size);

	/// <summary>
	/// Create and load a sphere, either into data.
	/// </summary>
	/// <param name="radius">The radius of the original vertices before transforms</param>
	/// <param name="slices">Number of slices to make (vertical)</param>
	/// <param name="stacks">Numver of stacks to make (horizontal)</param>
	int LoadDataForSphere(float radius, int slices, int stacks);

	// Setter/Getters for private variables.
	SETGET(int, Type);

	/// <summary>
	/// Render the geometry object.
	/// </summary>
	/// <param name="pShaderProgram">Shader to render the object</param>
	void Render(Shader *pShaderProgram);
};

