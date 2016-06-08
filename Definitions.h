#pragma once

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

struct Container {//Container: [X, Y, Z] [R, G, B] [S, T]
	//Position
	glm::vec3 vertex;
	//Normal
	glm::vec3 normal;
	//TexCoords
	glm::vec2 texCoord;
};

struct Texture {//Texture Container to hold certain textures.
	GLuint id;
	std::string type;
};

struct Material {//Oobject material
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct DirLight {//Directional Light
	bool on;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight {//Point Light
	bool on;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float quadratic;
};

struct SpotLight {//Spot Light
	bool on;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float quadratic;
	glm::vec3 direction;
	float spotCutoff;
	float spotExponent;
};

struct Struct_Camera {//Global Camera
	glm::vec3 position;
	glm::vec3 lookat;
	glm::vec3 up;
	glm::vec3 direction;
	glm::vec3 right;
};

#endif
