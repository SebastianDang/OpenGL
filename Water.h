//
//  Water.h
//  Project1
//
//  Created by Parth Patel on 5/27/16.
//  Copyright © 2016 Parth Patel. All rights reserved.
//

#ifndef Water_h
#define Water_h

#include "Window.h"
#include "Definitions.h"

//A struct to hold a control point of the surface.
struct Point {
	float x;
	float y;
	float z;
};

class Water
{
private:
	//Variables to keep track of information.
	std::vector<Container> containers;//[v, vn, (s,t)]
	std::vector<glm::vec3> vertices;//v
	std::vector<glm::vec3> normals;//vn
	std::vector<glm::vec2> texCoords;//(s,t)
	std::vector<unsigned int> indices;//indices.
	//GLSL properties.
	GLuint VAO, VBO, EBO;
	//Intialization functions.
	void setupGeometry();
	void setupWater();
	//Frame buffers for water reflection/refraction.
	void initializeReflection();
	void initializeRefraction();
	//Misc.
	int draw_mode;

public:
	//Constructor methods.
	Water(int x_d, int z_d);
    ~Water();
	//Determine the Water's position in the world.
	int x, z;
	glm::mat4 toWorld;
	//Draw and update methods.
	void toggleDrawMode();
	void draw(GLuint shaderProgram);
	//Frame buffers for water reflection/refraction.
	GLuint reflection_FBO, reflection_texture, reflection_DBO;
	GLuint refraction_FBO, refraction_texture, refraction_DBO;
};

#endif
