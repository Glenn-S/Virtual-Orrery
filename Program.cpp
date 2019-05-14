/*
 * Program.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 *  Modified by: Glenn Skelton
 *
 *  Images obtained from: https://www.solarsystemscope.com/textures/
 */

#include "Program.h"

#include <iostream>
#include <string>
#include <vector>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"

#include "RenderingEngine.h"
#include "Scene.h"
#include "TextureParser.h"
#include "Geometry.h"
#include "Camera.h"

Program *prog; // global pointer to the program

Program::Program() {
	setupWindow();

	renderingEngine = new RenderingEngine();


	// Upload all stellar data

	vector<Geometry> *sceneGraph = new vector<Geometry>();
	Geometry *obj = new Geometry();
	tp.fileParse("textures/sphere.obj", obj); // parse the file and store the data for the triangles in each respective object

	Geometry STARS = *obj;
	STARS.name = "Stars";
	STARS.planetaryRadius = 99.9f;
	InitializeTexture(&STARS.texture, "images/milkyWay.png", GL_TEXTURE_2D); // load and store image
	STARS.scale = glm::scale(glm::mat4x4(1), glm::vec3(STARS.planetaryRadius)); // set scaling
	STARS.translation = glm::translate(glm::mat4x4(1), glm::vec3(0.0f, 0.0f, 0.0f));


	Geometry SUN = *obj;
	SUN.name = "Sun";
	InitializeTexture(&SUN.texture, "images/sun.png", GL_TEXTURE_2D); // load and store image
	SUN.planetaryRadius = 695842.0f; // actual radius of the sun that will be scaled down
	SUN.period = 25.38f; // earth days
	SUN.orbitalPeriod = 0.0f; // no orbital period for Sun (1 so that sun does not disappear
	SUN.orbitalRadius = 0.0f; // it is the origin
	SUN.scale = glm::scale(glm::mat4x4(1), glm::vec3((std::log(SUN.planetaryRadius) / std::log(10000)))); // set scaling
	SUN.translation = glm::translate(glm::mat4x4(1), glm::vec3(0.0f, 0.0f, 0.0f));
	// origin is at the center of the scene

	Geometry EARTH = *obj;
	EARTH.name = "Earth";
	InitializeTexture(&EARTH.texture, "images/earth.png", GL_TEXTURE_2D); // load and store image
	EARTH.planetaryRadius = 6371.0f; // actual radius of the earth KM
	EARTH.period = 1.0f; // days
	EARTH.orbitalPeriod = 365.26; // earth days
	EARTH.orbitalRadius = 1.0f; // AU
	EARTH.scale = glm::scale(glm::mat4x4(1), glm::vec3((log(EARTH.planetaryRadius) / log(10000)))); // set scaling
	EARTH.translation = glm::translate(glm::mat4x4(1), glm::vec3(0.0f, 0.0f, -EARTH.orbitalRadius * 10));
	EARTH.axialTilt = glm::rotate(glm::mat4x4(1), glm::radians(23.5f), glm::vec3(0,0,1));
	EARTH.orbitalTilt = glm::rotate(glm::mat4x4(1), glm::radians(7.155f), glm::vec3(0,0,1)); // tilt the orbital path

	Geometry MOON = *obj;
	MOON.name = "Moon";
	InitializeTexture(&MOON.texture, "images/moon.png", GL_TEXTURE_2D); // load and store image
	MOON.planetaryRadius = 1737.0f; // actual radius of the moon
	MOON.period = -27.32f; // moon rotates at the same speed as it's period clockwise
	MOON.orbitalPeriod = 27.32f; // earth days
	MOON.orbitalRadius = 384400.0f / 149597871.0f; // relative to the earth KM / KM of AU
	MOON.scale = glm::scale(glm::mat4x4(1), glm::vec3(log(MOON.planetaryRadius) / log(10000))); // set scaling
	MOON.translation = glm::translate(glm::mat4x4(1), glm::vec3(0.0f, 0.0f, -MOON.orbitalRadius * 1000));
	MOON.axialTilt = glm::rotate(glm::mat4x4(1), glm::radians(1.54f), glm::vec3(0,0,1));
	MOON.orbitalTilt = glm::rotate(glm::mat4x4(1), glm::radians(5.0f), glm::vec3(0,0,1)); // tilt the orbital path

	Geometry MARS = *obj;
	MARS.name = "Mars";
	InitializeTexture(&MARS.texture, "images/mars.png", GL_TEXTURE_2D); // load and store image
	MARS.planetaryRadius = 3390.0f; // actual radius of mars
	MARS.period = 1.017f; // earth days
	MARS.orbitalRadius = 1.524f; // AU
	MARS.orbitalPeriod = 686.97f;
	MARS.scale = glm::scale(glm::mat4x4(1), glm::vec3((log(MARS.planetaryRadius) / log(10000)))); // set scaling
	MARS.translation = glm::translate(glm::mat4x4(1), glm::vec3(0.0f, 0.0f, -MARS.orbitalRadius * 10));
	MARS.axialTilt = glm::rotate(glm::mat4x4(1), glm::radians(25.0f), glm::vec3(0,0,1));
	MARS.orbitalTilt = glm::rotate(glm::mat4x4(1), glm::radians(5.65f), glm::vec3(0,0,1)); // tilt the orbital path

	// store all objects in scene graph
	EARTH.children.push_back(MOON); // child of the earth
	SUN.children.push_back(EARTH);
	SUN.children.push_back(MARS);


	// get the initial calculations for the camera view and projection matrix, will get adjusted in key callback each frame
	sceneGraph->push_back(STARS);
	sceneGraph->push_back(SUN);
	Camera *cam = new Camera(&SUN); // create a camera to give to the scene
	cam->updateView(glm::pi<float>()/2, glm::pi<float>()/2, 50.0f); // for testing updating the view matrix for camera
	cam->updateProj(45.0f);
	scene = new Scene(renderingEngine, sceneGraph, cam);

}

Program::~Program() {
	//Must be cleaned up in the destructor because these are allocated to the heap
	delete renderingEngine;
	delete scene;
}

void Program::start() {
	prog = this;
	startTime = glfwGetTime();
	curTime = 0.0f;

	//Main render loop
	while(!glfwWindowShouldClose(window)) {
		if (!stopTime) { // if pause is not pressed
			curTime = glfwGetTime();

			scene->time = (curTime - startTime) * timeMultiplier; // can have a multiplier for slowing down time
			prog->scene->generateScene(); // update the scene with the time rotations
		}

		scene->displayScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void Program::setupWindow() {
	//Initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
		return;
	}

	//Set the custom error callback function
	//Errors will be printed to the console
	glfwSetErrorCallback(ErrorCallback);

	//Attempt to create a window with an OpenGL 4.1 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 1024;
	int height = 1024;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}

	//Set the custom function that tracks key presses
	glfwSetKeyCallback(window, KeyCallback);

	//Set the custom function that tracks the mouse position
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseBtnCallback);
	glfwSetScrollCallback(window, scrollCallback);

	//Bring the new window to the foreground (not strictly necessary but convenient)
	glfwMakeContextCurrent(window);

	//Intialize GLAD (finds appropriate OpenGL configuration for your system)
	if (!gladLoadGL()) {
		std::cout << "GLAD init failed" << std::endl;
		return;
	}

	//Query and print out information about our OpenGL environment
	QueryGLVersion();
}



void Program::QueryGLVersion() {
	// query opengl version and renderer information
	std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// reset the solar system
	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		prog->startTime = glfwGetTime(); // reset the time
	}

	// start and stop the time
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		if (!prog->stopTime) {
			prog->stopTime = true;
			prog->elapsed = glfwGetTime();
		}
		else {
			prog->stopTime = false;
			prog->startTime += glfwGetTime() - prog->elapsed; //adjust start time to counter act pause
		}
	}

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	// update the scaling matrix
	if ( (prog->scene->cam->radius + yoffset > 1.5) && (prog->scene->cam->radius + yoffset < 99) ) {
		prog->scene->cam->updateView(prog->scene->cam->theta, prog->scene->cam->phi, prog->scene->cam->radius + yoffset);
		prog->scene->generateScene();
	}

}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	if (prog->pressed) {
		prog->scene->cam->updateView(prog->scene->cam->theta + glm::radians((xpos - prog->mouseX) / 250),
									 prog->scene->cam->phi + glm::radians((prog->mouseY - ypos) / 250),
									 prog->scene->cam->radius);
		prog->scene->generateScene(); // update the scene
	}
}

void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods) {
	//Scene* thisScene = program->getScene(); // get the scene
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	// left click will be used for drag camera
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    	prog->pressed = true;
    	prog->mouseX = xPos;
    	prog->mouseY = yPos;
    }
    // release to stop moving the camera
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    	prog->pressed = false;
    }
}
