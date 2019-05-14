/*
 * RenderingEngine.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 *
 *  Modified by: Glenn Skelton
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

#include "RenderingEngine.h"
#include "Camera.h"

#include <iostream>

//cpp file purposely included here because it just contains some global functions
#include "ShaderTools.h"

RenderingEngine::RenderingEngine() {
	shaderProgram = ShaderTools::InitializeShaders();
	if (shaderProgram == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}

}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::RenderScene(const std::vector<Geometry>& objects, Camera *cam) {
	//Clears the screen to a dark grey background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear for depth testing


	// go through all of the geometry in the scene
	for (const Geometry& g : objects) {
		glEnable(GL_DEPTH_TEST); // needed to make sure the depth is tested in the shader

		glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.texture.textureID);
		GLuint uniformLocation = glGetUniformLocation(shaderProgram, "imageTexture");
		glUniform1i(uniformLocation, 0);

		if(CheckGLErrors()) {
			std::cout << "Texture creation failed" << std::endl;
		}

		// set to know not to apply shaders to the sun or stars
		uniformLocation = glGetUniformLocation(shaderProgram, "shadeObj");
		if ((g.name.compare("Sun") == 0) || (g.name.compare("Stars") == 0)) glUniform1i(uniformLocation, 0);
		else glUniform1i(uniformLocation, 1);
		uniformLocation = glGetUniformLocation(shaderProgram, "shade");
		if ((g.name.compare("Sun") == 0) || (g.name.compare("Stars") == 0)) glUniform1i(uniformLocation, 0);
		else glUniform1i(uniformLocation, 1);

		// view matrix push back
		uniformLocation = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(cam->getView())); // load the view matrix in
		// projection matrix push back
		uniformLocation = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(cam->getProjection())); // load the projection matrix in
		// load in the model matrix for each object
		uniformLocation = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(g.modelMat)); // load the model matrix in


		// take care of shading
		// inspiration borrowed from the textbook for implementation
		glm::vec3 ka = glm::vec3(0.2, 0.2, 0.2); // ambient component
		glm::vec3 kd = glm::vec3(0.9, 0.9, 0.8); // diffues component
		glm::vec3 ks = glm::vec3(1.0, 1.0, 1.0); // specular component
		float phongExp = 12.0f;

		uniformLocation = glGetUniformLocation(shaderProgram, "ka");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(ka));
		uniformLocation = glGetUniformLocation(shaderProgram, "kd");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(kd));
		uniformLocation = glGetUniformLocation(shaderProgram, "ks");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(ks));
		uniformLocation = glGetUniformLocation(shaderProgram, "phongExponent");
		glUniform1f(uniformLocation, phongExp);


		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());
		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);

	//Generate vbos for the object
	//Constant 1 means 1 vbo is being generated
	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0); // second value was 3
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0); // second value was 3
	glEnableVertexAttribArray(2);

}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer); // store vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW); // was vec3

	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer); // store normals
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW); // was vec3

}

void RenderingEngine::deleteBufferData(Geometry& geometry) {
	glDeleteBuffers(1, &geometry.vertexBuffer);
	glDeleteBuffers(1, &geometry.normalBuffer);
	glDeleteBuffers(1, &geometry.colorBuffer);
	glDeleteBuffers(1, &geometry.uvBuffer);
	glDeleteVertexArrays(1, &geometry.vao);
}

bool RenderingEngine::CheckGLErrors() {
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}
