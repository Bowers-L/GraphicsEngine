#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

enum Direction {
	FORWARD,
	BACK, 
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.5f;
const float ZOOM = 45.0f;

class _3DCamera {
public:
	//defaults


	//coordinate system
	glm::vec3 Position;

	//rotation
	float Yaw;
	float Pitch;

	//camera settings
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	_3DCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
	~_3DCamera();

	glm::mat4 getViewMatrix();

	void processKeyboard(Direction dir, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset);
	void processMouseScroll(float yOffset);

	glm::vec3 getFront();
	glm::vec3 getUp();
	glm::vec3 getRight();
	glm::vec3 getWorldUp();

	void setPosRot(glm::vec3 pos, float yaw, float pitch);

private:	
	glm::vec3 Front;		
	glm::vec3 Up;		
	glm::vec3 Right;		
	glm::vec3 WorldUp;

	void updateCameraVectors();
};