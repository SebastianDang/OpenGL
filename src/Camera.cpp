#include "stdafx.h"
#include "Camera.h"

Camera::Camera(glm::vec3 e, glm::vec3 d, glm::vec3 up)
{
	Update(e, d, up);
}

void Camera::Update(glm::vec3 e, glm::vec3 d, glm::vec3 up)
{
	// Get 3 variables for the camera.
	m_Position = e;
	m_Lookat = d;
	m_Up = up;

	//Set up the rest of the camera.
	m_Direction = glm::normalize(m_Position - m_Lookat);
	m_Right = glm::normalize(glm::cross(m_Up, m_Direction));
}

void Camera::Update(GLfloat yaw, GLfloat pitch, GLfloat roll)
{
	// Setup yaw and pitch values.
	m_Yaw = yaw;
	m_Pitch = pitch;
	m_Roll = roll;
}

glm::mat4 Camera::GetMatrix()
{
	return glm::lookAt(m_Position, m_Lookat, m_Up);
}

void Camera::Translate(glm::vec3 DeltaFrameTime)
{
	// TODO: Implement
}

void Camera::Rotate(glm::vec3 axis, float angle)
{
	// TODO: Implement
}

void Camera::Invert(float distance)
{
	m_Position.y -= distance;
	Update(m_Position, m_Lookat, m_Up);
}

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 follow)
{
	m_FollowPoint = follow;
	glm::vec3 camera_position = glm::vec3(m_FollowPoint.x, m_FollowPoint.y, m_FollowPoint.z + INITIAL_CAMERA_DISTANCE);
	glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	Update(camera_position, m_FollowPoint, camera_up);
}

void ThirdPersonCamera::RotateAroundPoint(glm::vec3 point, glm::vec3 v, glm::vec3 w, float delta)
{
	// Set the look at point.
	m_Lookat = point;

	// Calculate the translation.
	glm::vec3 translate_v = glm::vec3(w.x - v.x, v.y - w.y, 0.0f);//Since x- and x+ are from left to right, y must be inverted so that y- and y+ are from bottom to top.
	translate_v *= (delta > 0.0) ? delta : MOUSE_SENSITIVITY;

	// Get the old position values for x (the distance from the lookat, not the origin).
	glm::vec3 cur_position_x = m_Position - m_Lookat;
	float cur_position_length_x = glm::length(cur_position_x);

	// Calculate the new (x, z) position (added to the lookat for proper position).
	glm::vec3 position_x = translate_v.x*m_Right;
	glm::vec3 new_position_x = cur_position_x + position_x;
	new_position_x = m_Lookat + (cur_position_length_x * glm::normalize(new_position_x));

	// Update the new camera position for x.
	m_Position = new_position_x;

	// Get the old position values for y (the distance from the lookat, not the origin).
	glm::vec3 cur_position_y = m_Position - m_Lookat;
	float cur_position_length_y = glm::length(cur_position_y);

	// Calculate the new (y) position.
	glm::vec3 position_y = translate_v.y*m_Up;
	glm::vec3 new_position_y = cur_position_y + position_y;
	new_position_y = m_Lookat + (cur_position_length_y * glm::normalize(new_position_y));
	glm::vec3 new_position_distance = new_position_y - m_Lookat;

	// Check to make sure it doesn't hit a max.
	glm::vec3 horizontal_pos = glm::vec3(m_Position.x, 0.0f, m_Position.z);
	glm::vec3 horizontal_lookat = glm::vec3(m_Lookat.x, 0.0f, m_Lookat.z);
	glm::vec3 horizontal_distance = horizontal_pos - horizontal_lookat;
	float angle = glm::dot(new_position_distance, horizontal_distance) / (glm::length(new_position_distance)*glm::length(horizontal_distance)); // Calculate the angle from the floor.

	// Update the new camera position for y. Check if the angle is greater than MIN_CAMERA_PITCH_Y and less than MAX_CAMERA_PITCH
	if (angle >= glm::radians(MAX_CAMERA_PITCH) && new_position_y.y >= MIN_CAMERA_PITCH_Y)
	{
		// If following an object, check to make sure we don't go below the height of the object.
		if (new_position_y.y <= MIN_CAMERA_PITCH_Y) { return; }

		// Update the camera to the new y position.
		m_Position = new_position_y;
	}

	// Update the other two vectors.
	Update(m_Position, m_Lookat, m_Up);
}

void ThirdPersonCamera::Zoom(double y)
{
	// Calculate the translation (Scale the zoom if necessary).
	float z = (float)y;

	// Calculate new camera distance.
	glm::vec3 direction = m_Direction;// position in relation to the origin.
	glm::vec3 displacement = direction * z;// Multiply Z so it moves in the correct direction (towards the origin).
	glm::vec3 zoom = m_Position - displacement;

	// Zoom is limited. Calculate threshold.
	if ((glm::distance(zoom, m_Lookat) > ZOOM_MIN_DISTANCE) && (glm::distance(zoom, m_Lookat) < ZOOM_MAX_DISTANCE))
	{
		m_Position = zoom;
	}

	// Update the other two vectors.
	Update(m_Position, m_Lookat, m_Up);
}

FirstPersonCamera::FirstPersonCamera(glm::vec3 e)
{
	m_Position = e;
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	CalculateView();
}

void FirstPersonCamera::CalculateView()
{
	// Calculate the new direction to look in.
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));

	m_Lookat = m_Position + glm::normalize(direction);

	// Update the other two vectors.
	Update(m_Position, m_Lookat, m_Up);
}

void FirstPersonCamera::Move(glm::vec3 v, glm::vec3 w, float delta)
{
	// Calculate the translation with proper mouse sensitivity.
	glm::vec3 translate_v = glm::vec3(w.x - v.x, v.y - w.y, 0.0f); // Since x- and x+ are from left to right, y must be inverted so that y- and y
	translate_v *= (delta > 0.0) ? delta : MOUSE_SENSITIVITY;

	// Update the pitch and yaw values.
	AdjustPitch(translate_v.y);
	AdjustYaw(translate_v.x);

	// Update the other two vectors.
	CalculateView();
}

void FirstPersonCamera::AdjustPitch(float delta)
{
	// Update the pitch value
	float pitch = m_Pitch + delta;

	// Constrain the pitch to avoid unwanted errors.
	if (pitch > MAX_PITCH) pitch = MAX_PITCH;
	if (pitch < MIN_PITCH) pitch = MIN_PITCH;

	m_Pitch = pitch;
}

void FirstPersonCamera::AdjustYaw(float delta)
{
	// Update the pitch value
	float yaw = m_Yaw + delta;

	// Adjust the yaw to avoid unwanted errors (limits, etc).
	if (yaw <= (INITIAL_YAW - 360.0f) || yaw >= (INITIAL_YAW + 360.0f)) yaw = INITIAL_YAW;

	// Adjust if we want to lock the position.
	if (m_LockFieldOfView)
	{
		// Lock accordingly.
		if (yaw <= m_FieldOfViewMinAngle) yaw = m_FieldOfViewMinAngle;
		if (yaw >= m_FieldOfViewMaxAngle) yaw = m_FieldOfViewMaxAngle;
	}

	m_Yaw = yaw;
}

void FirstPersonCamera::LockFieldOfView(glm::vec3 direction, float fov)
{
	// First we calculate the angle according to the direction.
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f); // This is assuming default -z direction of view.
	float angle = glm::dot(glm::normalize(forward), glm::normalize(direction));

	// Get restricted angles.
	float leftAngle = (INITIAL_YAW + angle) - (fov / 2.0f);
	float rightAngle = (INITIAL_YAW + angle) + (fov / 2.0f);

	// Update.
	m_FieldOfViewMinAngle = leftAngle;
	m_FieldOfViewMaxAngle = rightAngle;
	m_FieldOfView = fov;
	m_LockFieldOfView = true;
}

void FirstPersonCamera::UnlockFieldOfView()
{
	// Reset all
	m_FieldOfViewMinAngle = 0.0f;
	m_FieldOfViewMaxAngle = 0.0f;
	m_FieldOfView = 360.0f;
	m_LockFieldOfView = false;
}
