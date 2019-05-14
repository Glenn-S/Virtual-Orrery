/*
 * Geometry.h
 *	Class for storing geometry
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 *  Modified by: Glenn Skelton
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"

using namespace std;

class Geometry {
public:
	Geometry();
	virtual ~Geometry();

	//Data structures for storing vertices, normals colors and uvs
	std::vector<glm::vec4> verts;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> uvs;

	//Pointers to the vao and vbos associated with the geometry
	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint colorBuffer;

	//Draw mode for how OpenGL interprets primitives
	GLuint drawMode;


	std::vector<Geometry> children; // list of children

	// object texture
	std::string name;
	MyTexture texture; // associated image
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);

	// associated matrices and measurements for model
	float planetaryRadius = 1.0f; // KM
	float period = 1.0f; // earth days
	float orbitalPeriod = 0.0f; // earth days
	float orbitalRadius = 0.0f; // AU

	glm::mat4x4 translation = glm::mat4x4(1); // distance from the central mass
	glm::mat4x4 planetaryRot = glm::mat4x4(1); // planets own rotation
	glm::mat4x4 orbitalRot = glm::mat4x4(1); // rotation around the sun
	glm::mat4x4 axialTilt = glm::mat4x4(1); // tilt of the planet
	glm::mat4x4 orbitalTilt = glm::mat4x4(1); // tilt of the planets orbit
	glm::mat4x4 scale = glm::mat4x4(1); // size of the celestial body
	glm::mat4x4 modelMat = glm::mat4x4(1); // model matrix to store it all


};

#endif /* GEOMETRY_H_ */
