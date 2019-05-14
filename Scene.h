/*
 * Scene.h
 *	Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 *  Modified by: Glenn Skelton
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include "Geometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;
class Camera;

class Scene {
public:
	Scene(RenderingEngine* renderer, vector<Geometry>* sceneGraph, Camera *cam);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();
	void generateScene();
	void traverseGraph(Geometry *obj, glm::mat4x4 parentTranslation, double time);

	std::vector<Geometry> *sceneGraph = NULL; // for storing the objects of my scene

	Camera *cam = NULL;
	double time = 0.0f; // for rotational matrices
	bool initiallized = false;

private:
	RenderingEngine* renderer;

	//list of objects in the scene
	std::vector<Geometry> objects;
};

#endif /* SCENE_H_ */
