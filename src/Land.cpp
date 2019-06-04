#include "stdafx.h"
#include "Land.h"

Land::Land()
{
}

Land::~Land()
{
}

glm::vec3 Land::CalculateNormalFromTrianglePoints(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	// Construct the 2 lines of the triangle.
	glm::vec3 p01 = p1 - p0;
	glm::vec3 p02 = p2 - p0;

	// Perform the cross product.
	glm::vec3 normal =  glm::cross(p01, p02);

	// Normalize the vector.
	return glm::normalize(normal);
}
