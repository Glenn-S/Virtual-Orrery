/**
 * Camera.h
 *
 * Glenn Skelton
 */


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

#include "Geometry.h"

class Camera {
public:

	Camera(Geometry *obj);
	~Camera();


	float radius = 10.0f;
	float theta = 0;
	float phi = 0.01;

	float fov = 45.0f; // angle in Radians
	float aspect = 1.0f; // the aspect ratio for the perspective matrix // will adjust this later to be general
	float nearPlane = 0.1f; // near plane
	float farPlane = 200.0f;  // far plane


	Geometry *centered; // the object which the camera is parented to

	// maybe store some matrices too? view, projection etc
	glm::mat4x4 getView();
	glm::mat4x4 getProjection();

	void updateView(float theta, float phi, float radius);
	void updateProj(float fov);

	glm::mat4x4 view;
	glm::mat4x4 proj;

private:
	glm::vec3 eye;
	glm::vec3 at = glm::vec3(0.0f, 0.0f, 0.0); // center around the center of the solar system (calculated from centered)
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // y is my up vector


};
