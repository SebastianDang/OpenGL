#pragma once
#include "Object.h"

/// <summary>
/// Class to create simple shapes.
/// </summary>
class Geo_Object : public Object
{
private:

public:
	Geo_Object();
	~Geo_Object();

	/// <summary>
	/// Create and load a cube into the buffer.
	/// </summary>
	void LoadCube();

	/// <summary>
	/// Create and load a sphere into the buffer.
	/// </summary>
	/// <param name="radius"></param>
	/// <param name="slices"></param>
	/// <param name="stacks"></param>
	void LoadSphere(float radius, int slices, int stacks);

	/// <summary>
	/// Render the geometry object.
	/// </summary>
	/// <param name="pShaderProgram">Shader to render the object</param>
	void Render(Shader *pShaderProgram);
};

