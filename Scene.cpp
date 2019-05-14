/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 *
 *  Modified by: Glenn Skelton
 */

#include "Scene.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <exception>

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"
#include "TextureParser.h"
#include "Geometry.h"
#include "Camera.h"

using namespace std;

Scene::Scene(RenderingEngine* renderer, vector<Geometry>* sceneGraph, Camera *cam) : renderer(renderer) {
	this->cam = cam; // load the camera
	this->sceneGraph = sceneGraph; // load the scene graph

	generateScene();
	this->initiallized = true;
}


/**
 * Purpose: To generate a scene in the solar system. Not finished being implemented
 */
void Scene::generateScene() {
	objects.clear(); // clear the scene

	// load the objects of the scene in

	traverseGraph(&sceneGraph->at(1), glm::mat4x4(1), this->time);

	// load the background image in
	if (!this->initiallized) {
		sceneGraph->at(0).modelMat = sceneGraph->at(0).translation * sceneGraph->at(0).scale;
		sceneGraph->at(0).drawMode = GL_TRIANGLES;
		// this is the memory issue
		RenderingEngine::assignBuffers(sceneGraph->at(0)); // call only once
		RenderingEngine::setBufferData(sceneGraph->at(0));
	}

	objects.push_back(sceneGraph->at(0));
}


void Scene::traverseGraph(Geometry *obj, glm::mat4x4 parentTranslation, double time) { // add time in here to update
	// update transformations
	obj->planetaryRot = glm::rotate(glm::mat4x4(1), (((2*glm::pi<float>()) / obj->period) * (float)time), glm::vec3(0,1,0)); // update the celestial bodies rotation

	if (obj->name.compare("Sun") == 0) obj->orbitalRot = glm::mat4x4(1); // take care of the fact that the sun does not have a orbital period
	else obj->orbitalRot = glm::rotate(glm::mat4x4(1), (((2*glm::pi<float>()) / obj->orbitalPeriod) * (float)time), glm::vec3(0,1,0)); // update the celestial bodies orbit

	// create the model matrix for each stellar body
	obj->modelMat = parentTranslation *
					obj->orbitalRot *
					obj->orbitalTilt *
					obj->translation *
					obj->axialTilt *
					obj->planetaryRot *
					obj->scale;

	if (obj->children.size() > 0) {
		for (unsigned int i = 0; i < obj->children.size(); i++) {
			traverseGraph(&obj->children.at(i), (obj->orbitalRot * obj->translation), this->time); // go deeper, adding the translations together
		}
	}

	if (!this->initiallized) {
		obj->drawMode = GL_TRIANGLES;
		RenderingEngine::assignBuffers(*obj);
		RenderingEngine::setBufferData(*obj);
	}

	objects.push_back(*obj);
}


Scene::~Scene() {
	delete sceneGraph;
	delete cam;
}

void Scene::displayScene() {
	renderer->RenderScene(objects, this->cam);
}

