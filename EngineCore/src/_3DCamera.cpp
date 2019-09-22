#include "_3DCamera.h"

_3DCamera::_3DCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM)
{
	this->Position = position;
	this->WorldUp = up;

	this->Yaw = yaw;
	this->Pitch = pitch;

	updateCameraVectors();
}

_3DCamera::~_3DCamera()
{
}

glm::mat4 _3DCamera::getViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void _3DCamera::processKeyboard(Direction dir, float deltaTime)
{
	float vel = MovementSpeed * deltaTime;
	switch (dir) {
	case FORWARD:
		Position += Front * vel;
		break;
	case BACK:
		Position -= Front * vel;
		break;
	case LEFT:
		Position -= Right * vel;
		break;
	case RIGHT:
		Position += Right * vel;
		break;
	}
}

void _3DCamera::processMouseMovement(float xOffset, float yOffset)
{
	//calculate pitch and yaw based on mouse offset
	Yaw = xOffset * MouseSensitivity + YAW;
	Pitch = yOffset * MouseSensitivity + PITCH;

	if (Pitch > 89.0f) {
		Pitch = 89.0f;
	}
	if (Pitch < -89.0f) {
		Pitch = -89.0f;
	}

	updateCameraVectors();
}

void _3DCamera::processMouseScroll(float yOffset)
{
	//zooming
	if (Zoom >= 1.0f && Zoom <= 45.0f) {
		Zoom -= yOffset;
	}

	if (Zoom >= 45.0f) {
		Zoom = 45.0f;
	}
	if (Zoom <= 1.0f) {
		Zoom = 1.0f;
	}
}

void _3DCamera::updateCameraVectors()
{
	//calculate front vector based on pitch and yaw
	glm::vec3 front;

	front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	front.y = sin(glm::radians(Pitch));

	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::vec3 _3DCamera::getFront()
{
	return Front;
}

glm::vec3 _3DCamera::getUp()
{
	return Up;
}

glm::vec3 _3DCamera::getRight()
{
	return Right;
}

glm::vec3 _3DCamera::getWorldUp()
{
	return WorldUp;
}

void _3DCamera::setPosRot(glm::vec3 pos, float yaw, float pitch) {
	Position = pos;
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVectors();
}