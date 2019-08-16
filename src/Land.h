#pragma once

// Base class for representing any land type object. This will be for terrains, water, etc.
// Ideally this will be a Low Poly implementation.
class Land
{
private:

protected:

	// Calculate the normal from a set of 3 points, where right hand rule applies.
	// p0 -> p1 -> p2.
	static glm::vec3 CalculateNormalFromTrianglePoints(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);

public:
	Land();
	virtual ~Land();

};