#pragma once

#include <third-party/glm/glm.hpp>
#include <third-party/glm/gtc/matrix_transform.hpp>

struct Camera
{
	glm::vec3 position{ 0.0f, 0.0f, 3.0f }; // where is our camera in the world
	glm::vec3 target{ 0.0f, 0.0f, 0.0f }; // where it is looking at
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };    // which direction is "up" for the camera

	float fovDegrees{ 45.0f }; // field of view in degrees
	float aspectRatio{ 1.0f }; // width / height
	float nearClip{ 0.1f };    // near clipping plane, the nearest distance to the camera that will be rendered
	float farClip{ 100.0f };   // far clipping plane, the farthest distance from the camera that will be rendered

	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(position, target, up);
	}

	glm::mat4 GetProjectionMatrix() const
	{
		return glm::perspective(glm::radians(fovDegrees), aspectRatio, nearClip, farClip);
	}
};