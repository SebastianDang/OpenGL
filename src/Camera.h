#pragma once

#ifndef CAMERA_H
#define CAMERA_H

class Camera 
{

protected:
	glm::vec3 m_Position;
	glm::vec3 m_Lookat;
	glm::vec3 m_Up;

	glm::vec3 m_Direction;
	glm::vec3 m_Right;

	GLfloat m_Yaw = 0.0f;
	GLfloat m_Pitch = 0.0f;
	GLfloat m_Roll = 0.0f;

public:
	Camera() {}
	virtual ~Camera() {}
	Camera(glm::vec3 e, glm::vec3 d, glm::vec3 up);

	void Update(glm::vec3 e, glm::vec3 d, glm::vec3 up);
	void Update(GLfloat yaw, GLfloat pitch, GLfloat roll);

	glm::mat4 GetMatrix();

	void Translate(glm::vec3 DeltaFrameTime);
	void Rotate(glm::vec3 axis, float angle);
	void Invert(float distance); // Positive to invert, negative to revert back.
};

#define INITIAL_CAMERA_DISTANCE 20.0f // Set the initial camera distance from an object.
#define MAX_CAMERA_PITCH 20.0f // NEGATIVE ANGLE. Keep this at 20.0f for now.
#define MIN_CAMERA_PITCH_Y 0.0f // Y position at the bottom. Can't go below this.
#define ZOOM_MIN_DISTANCE 2.0f // Minimum zoom distance.
#define ZOOM_MAX_DISTANCE 40.0f // Maximum zoom distance.

class ThirdPersonCamera : public Camera
{
private:
	glm::vec3 m_FollowPoint;

public:
	ThirdPersonCamera(glm::vec3 follow);

	SETGET(glm::vec3, FollowPoint);
	void RotateAroundPoint(glm::vec3 point, glm::vec3 v, glm::vec3 w, float delta = 0.0f);
	void Zoom(double y);

};

#define INITIAL_YAW 0.0f
#define INITIAL_PITCH 0.0f
#define MIN_PITCH -89.0f
#define MAX_PITCH 89.0f

class FirstPersonCamera : public Camera
{
private:

	// If we want to lock the field of view to a certain angle and direction.
	bool m_LockFieldOfView = false;
	float m_FieldOfView = 360.0f;
	float m_FieldOfViewMinAngle = 0.0f;
	float m_FieldOfViewMaxAngle = 0.0f;

	void CalculateView();

public:
	FirstPersonCamera(glm::vec3 e);

	void Move(glm::vec3 v, glm::vec3 w, float delta = 0.0f);
	void AdjustPitch(float delta);
	void AdjustYaw(float delta);

	void LockFieldOfView(glm::vec3 direction, float fov);
	void UnlockFieldOfView();
};
#endif