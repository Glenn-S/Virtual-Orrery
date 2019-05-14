/**
 * TextureParser.h
 *
 * For parsing each texture file for the spheres
 *
 * Glenn Skelton
 */

#include <iostream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Geometry;

using namespace std;


#ifndef TEXTURE_PARSER_H
#define TEXTURE_PARSER_H

class TextureParser {
public:
	TextureParser();
	~TextureParser();

	void fileParse(string filename, Geometry *obj);
	void printVertices(vector<vector<glm::vec3>> triangles);
private:
};

#endif /* TEXTURE_PARSER_H */
