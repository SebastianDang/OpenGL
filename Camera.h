#pragma once

#include "Window.h"
#include "Definitions.h"
#include "OBJObject.h"

#ifndef CAMERA_H
#define CAMERA_H
class Camera {
private:
	//Camera variables
	Struct_Camera camera;
	Struct_Camera saved_camera;

	//Eular Angles.
	GLfloat yaw;
	GLfloat pitch;
	GLfloat saved_yaw;
	GLfloat saved_pitch;

	//Object to follow for third person.
	OBJObject * toFollow;
	
	//Setup function.
	void setupCamera(glm::vec3 e, glm::vec3 d, glm::vec3 up);
	
	//Updates the corresponding camera vectors.
	void updateCamera();

public:
	Camera(glm::vec3 e, glm::vec3 d, glm::vec3 up);
	Camera(OBJObject * object_follow);
	~Camera();

	//Update the main window class with current camera coordinates.
	void window_updateCamera();

	//Functions for following an object.
	void object_follow();
	void third_person_rotate(glm::vec3 v, glm::vec3 w);
	void third_person_zoom(double y);

	//Functions for first person view.
	void first_person_movement(glm::vec3 v, glm::vec3 w);

	//Function to switch between first person and third person view.
	void save();
	void load();
};
#endif