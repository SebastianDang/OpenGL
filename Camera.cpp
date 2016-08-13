#include "Camera.h"
#include "OBJObject.h"

using namespace std;

#define INITIAL_CAMERA_DISTANCE 20.0f//Set the initial camera distance from an object.
#define MAX_CAMERA_PITCH 20.0f//NEGATIVE ANGLE. Keep this at 20.0f for now.
#define MIN_CAMERA_PITCH_Y 0.0f//Y position at the bottom. Can't go below this.
#define ZOOM_MIN_DISTANCE 2.0f//Minimum zoom distance.
#define ZOOM_MAX_DISTANCE 40.0f//Maximum zoom distance.
#define MOUSE_SENSITIVITY 0.001f//Mouse sensivity for FPS style camera.
#define INITIAL_YAW -90.0f
#define INITIAL_PITCH 0.0f
#define MAX_PITCH 89.0f
#define MIN_PITCH -89.0f

/* Setup the camera for arbritrary view, not attached to an object. */
Camera::Camera(glm::vec3 e, glm::vec3 d, glm::vec3 up)
{
	this->setupCamera(e, d, up);
}

/* Setup the camera for 3rd person view. */
Camera::Camera(OBJObject * object_follow)
{
	this->toFollow = object_follow;//Object to be followed.
	glm::vec3 object_position = glm::vec3(object_follow->toWorld[3]);//Get the object's current position.
	glm::vec3 camera_position = glm::vec3(object_position.x, object_position.y, object_position.z + INITIAL_CAMERA_DISTANCE);
	glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->setupCamera(camera_position, object_position, camera_up);
}

/* Deconstructor to safely delete when finished. */
Camera::~Camera()
{

}

/* Setup camera for the object. */
void Camera::setupCamera(glm::vec3 e, glm::vec3 d, glm::vec3 up) {
	//Get 3 variables for the camera.
	this->camera.position = e;
	this->camera.lookat = d;
	this->camera.up = up;
	//Set up the rest of the camera.
	this->camera.direction = glm::normalize(camera.position - camera.lookat);
	this->camera.right = glm::normalize(glm::cross(camera.up, camera.direction));
	//Setup yaw and pitch values.
	this->yaw = INITIAL_YAW;
	this->pitch = INITIAL_PITCH;
	//Save the current values.
	this->save();
}

/* Update the camera variables. */
void Camera::updateCamera() {
	this->camera.direction = glm::normalize(camera.position - camera.lookat);
	this->camera.right = glm::normalize(glm::cross(camera.up, camera.direction));
}

/* [Window Class] Update the camera, based on the current camera struct. Called whenever we want to switch to this camera. */
void Camera::window_updateCamera()
{
	Window::camera_update_callback(this->camera.position, this->camera.lookat, this->camera.up);
}

/* Changes the position and lookat to follow an object. */
void Camera::object_follow()
{
	//Do nothing if there is no object.
	if (!toFollow)
	{
		return;
	}
	//Get the object's position and camera's current lookat position.
	glm::vec3 object_position = glm::vec3(this->toFollow->toWorld[3]);
	glm::vec3 camera_position = this->camera.lookat;
	//Calculate the displacement between the two.
	glm::vec3 displacement = object_position - camera_position;
	//Update the camera vectors accordingly.
	this->camera.lookat += displacement;
	this->camera.position += displacement;
	this->updateCamera();
}

/* Rotate around the middle of the screen (most likely an object) based on mouse drag from v to w. */
void Camera::third_person_rotate(glm::vec3 v, glm::vec3 w)
{
	this->camera.lookat = glm::vec3(this->toFollow->toWorld[3]);
	//Calculate the translation.
	glm::vec3 translate_v = glm::vec3(w.x - v.x, v.y - w.y, 0.0f);//Since x- and x+ are from left to right, y must be inverted so that y- and y+ are from bottom to top.
	translate_v *= Window::delta;
	//translate_v *= (float)(1 / (float)Window::width);//Scale the translation.
	//Get the old position values for x (the distance from the lookat, not the origin).
	glm::vec3 cur_position_x = this->camera.position - this->camera.lookat;
	float cur_position_length_x = glm::length(cur_position_x);
	//Calculate the new (x, z) position (added to the lookat for proper position).
	glm::vec3 position_x = translate_v.x*this->camera.right;
	glm::vec3 new_position_x = cur_position_x + position_x;
	new_position_x = this->camera.lookat + (cur_position_length_x * glm::normalize(new_position_x));
	//Update the new camera position for x.
	this->camera.position = new_position_x;
	//Get the old position values for y (the distance from the lookat, not the origin).
	glm::vec3 cur_position_y = this->camera.position - this->camera.lookat;
	float cur_position_length_y = glm::length(cur_position_y);
	//Calculate the new (y) position.
	glm::vec3 position_y = translate_v.y*this->camera.up;
	glm::vec3 new_position_y = cur_position_y + position_y;
	new_position_y = this->camera.lookat + (cur_position_length_y * glm::normalize(new_position_y));
	glm::vec3 new_position_distance = new_position_y - this->camera.lookat;
	//Check to make sure it doesn't hit a max.
	glm::vec3 horizontal_pos = glm::vec3(this->camera.position.x, 0.0f, this->camera.position.z);
	glm::vec3 horizontal_lookat = glm::vec3(this->camera.lookat.x, 0.0f, this->camera.lookat.z);
	glm::vec3 horizontal_distance = horizontal_pos - horizontal_lookat;
	float angle = glm::dot(new_position_distance, horizontal_distance) / (glm::length(new_position_distance)*glm::length(horizontal_distance));//Calculate the angle from the floor.
	//Update the new camera position for y. Check if the angle is greater than MIN_CAMERA_PITCH_Y and less than MAX_CAMERA_PITCH
	if (angle >= glm::radians(MAX_CAMERA_PITCH) && new_position_y.y >= MIN_CAMERA_PITCH_Y)
	{
		//If following an object, check to make sure we don't go below the height of the object.
		if (toFollow != nullptr && new_position_y.y <= MIN_CAMERA_PITCH_Y){ return; }
		//Update the camera to the new y position.
		this->camera.position = new_position_y;
	}
	//Update the remaining camera vectors.
	this->updateCamera();
}

/* Translate the object in camera space (z). */
void Camera::third_person_zoom(double y)
{
	//Calculate the translation (Scale the zoom if necessary).
	float z = (float)y;
	//Calculate new camera distance.
	glm::vec3 direction = this->camera.direction;//position in relation to the origin.
	glm::vec3 displacement = direction * z;//Multiply Z so it moves in the correct direction (towards the origin).
	glm::vec3 zoom = this->camera.position - displacement;
	//Zoom is limited. Calculate threshold.
	if ((glm::distance(zoom, camera.lookat) > ZOOM_MIN_DISTANCE) && (glm::distance(zoom, camera.lookat) < ZOOM_MAX_DISTANCE))
	{
		this->camera.position = zoom;
	}
	//Update the remaining camera vectors.
	this->updateCamera();
}

/* Rotate around the current position to allow a first person view with the camera. */
void Camera::first_person_movement(glm::vec3 v, glm::vec3 w)
{
	//Calculate the translation with proper mouse sensitivity.
	glm::vec3 translate_v = glm::vec3(w.x - v.x, v.y - w.y, 0.0f);//Since x- and x+ are from left to right, y must be inverted so that y- and y
	translate_v *= MOUSE_SENSITIVITY;
	//Update the pitch and yaw values.
	this->yaw += translate_v.x;
	this->pitch += translate_v.y;
	//Constrain the pitch to avoid unwanted errors.
	if (this->pitch > MAX_PITCH)
	{
		this->pitch = MAX_PITCH;
	}
	if (this->pitch < MIN_PITCH)
	{
		this->pitch = MIN_PITCH;
	}
	//Adjust the yaw to avoid unwanted errors (limits, etc).
	if (this->yaw <= (INITIAL_YAW - 360.0f) || this->yaw >= (INITIAL_YAW + 360.0f))
	{
		this->yaw = INITIAL_YAW;
	}
	//Calculate the new direction to look in.
	glm::vec3 direction;
	direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = sin(glm::radians(pitch));
	direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	this->camera.lookat = this->camera.position + glm::normalize(direction);
	this->updateCamera();
}

/* Save the current camera components. */
void Camera::save()
{
	this->saved_camera = this->camera;
	this->saved_pitch = this->pitch;
	this->saved_yaw = this->yaw;
}

/* Load the last saved camera components. */
void Camera::load()
{
	this->camera = this->saved_camera;
	this->pitch = this->saved_pitch;
	this->yaw = this->saved_yaw;
}

/* Alter the camera to render from an inverted position. */
void Camera::invert(float distance)
{
	this->camera.position.y -= distance;
	this->window_updateCamera();
}

/* Alter the camera back to render from the normal position. */
void Camera::revert(float distance)
{
	this->camera.position.y += distance;
	this->window_updateCamera();
}