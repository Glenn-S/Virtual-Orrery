/**
 * Camera.cpp
 *
 * Glenn Skelton
 */


#include "Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"



Camera::Camera(Geometry *obj) {
	this->centered = obj;
}

Camera::~Camera() {

}

/**
 * Purpose: To get the camera view matrix
 */
glm::mat4x4 Camera::getView() {
	return this->view;
}

/**
 * Purpose: To get the camera projection matrix
 */
glm::mat4x4 Camera::getProjection() {
	return this->proj;
}

/**
 * Purpose: To update the view matrix with the parameters passed in
 */
void Camera::updateView(float theta, float phi, float radius) {

	if (0 < glm::degrees(phi) && glm::degrees(phi) < 180) { // restrict so that you can't flip over to the other side
		this->phi = phi;
	}
	this->theta = theta; // update values to store
	this->radius = radius;

	// recalculate the eye basis
	this->eye = glm::vec3(this->radius * std::cos(this->theta) * std::sin(this->phi),
						  this->radius * std::cos(this->phi),
						  this->radius * std::sin(this->theta) * std::sin(this->phi)); // z value in my own perspective


	this->view = glm::lookAt(this->eye, this->at, this->up); // update what is stored

}

/**
 * Purpose: To update the projection matrix with the fov passed in.
 */
void Camera::updateProj(float fov) {
	this->proj = glm::perspective(glm::radians(this->fov), this->aspect, this->nearPlane, this->farPlane);
}







