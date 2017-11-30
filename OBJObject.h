#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#include "Window.h"
#include "Definitions.h"

// This class is used for generic objects loaded from objs.
// It will handle simple objs without any textures (yet).
class OBJObject
{
private:
	// The container is structured to hold vertices, normals, then texture coordinates in a single structure.
	// The layout is [x, y, z][xn, yn, zn][s, t]
	// For now : vertices, normals, and texCoords can be stored in a vector, but everything is stored in the container itself.
	// Indices are faces loaded when an obj file is parsed to properly interpolate through triangulated objs.

	std::vector<Container> containers;		// [v, vn, (s,t)]
	std::vector<glm::vec3> vertices;		// v
	std::vector<glm::vec3> normals;			// vn
	std::vector<glm::vec2> texCoords;		// vt
	std::vector<unsigned int> indices;		// f
	std::vector<Texture> textures;			// List of textures

	GLuint VAO, VBO, EBO;					// OpenGL BUffer Objects
	
	Material objMaterial;					// Material
	int material;							// Material selection

	float min_X, min_Y, min_Z;				// Variables for normalizing when parsing.
	float max_X, max_Y, max_Z; 
	float avg_X, avg_Y, avg_Z, scale_v;

	// Parse the object to create it.
	int parse(const char* filepath);

	// Setup initial object materials, lighting.
	void setupObject();
	void setupMaterial();

	// Update object properties using these. This typically will be called in the draw callback.
	void updateMaterial(GLuint shaderProgram);

	// Handling Object movement.
	glm::vec3 currentDirection;
	float currentRunSpeed;
	float currentTurnSpeed;

public:
	/* Object constructor and setups */
	OBJObject(const char* filepath, int material);
	~OBJObject();

	// Keep track of world in relation to the object.
	glm::mat4 toWorld;

	// Draw.
	void draw(GLuint shaderProgram, glm::vec4 clipPlane = glm::vec4(0.0f));

	// Apply any transformations to the objects
	void translate(glm::vec3 translation);
	void rotate(float degree, glm::vec3 axis);

	// Handling Object movement.
	void setRunSpeed(float speed) { currentRunSpeed = speed; }
	void setTurnSpeed(float speed) { currentTurnSpeed = speed; }
	void W_movement(glm::vec2 boundaries);
	void A_movement(glm::vec2 boundaries);
	void S_movement(glm::vec2 boundaries);
	void D_movement(glm::vec2 boundaries);
	void update_height(float height);

};

#endif