/**
 * TextureParser.cpp
 *
 * For parsing the texture files for each sphere
 *
 * Glenn Skelton
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <exception>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"


//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextureParser.h"
#include "Geometry.h"

using namespace std;


TextureParser::TextureParser() {

}
TextureParser::~TextureParser() {

}

/**
 * Read filename in and parse into the passed in structures
 */
void TextureParser::fileParse(string filename, Geometry *obj) {
	vector<glm::vec3> verts;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	vector<glm::vec3> textures;
	//string type;
	glm::vec3 vert = glm::vec3();
	glm::vec2 uv = glm::vec2();
	glm::vec3 normal = glm::vec3();
	int t1, t2, t3, t4, t5, t6, t7, t8, t9;

	int buffSize = 80;
	char buffer[buffSize]; // standard line length
	int countVerts = 0, countUVS = 0, countNorms = 0, countF = 0;

	try {
		ifstream infile(filename);

		while (!infile.eof()) {
			infile.getline(buffer, buffSize);
			if (buffer[0] == '#') {
				// ignore comments
				continue;
			}
			else if (buffer[0] == 'v') { // perhaps an issue in here?
				if (strstr(buffer, "vt") != NULL) {
					sscanf(buffer, "vt %f %f", &uv.x, &uv.y);
					//cout << to_string(uv) << endl;
					uvs.push_back(uv);
					countUVS++;
				}
				else if (strstr(buffer, "vn") != NULL) {
					sscanf(buffer, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
					//cout << to_string(normal) << endl;
					normals.push_back(normal);
					countNorms++;
				}
				else {
					sscanf(buffer, "v %f %f %f", &vert.x, &vert.y, &vert.z);
					//cout << to_string(vert) << endl;
					verts.push_back(vert);
					countVerts++;
				}
			}
			else if (buffer[0] == 'f') {
				// just parse in here, don't bother going back, maybe pass pointer to geometry and put data in

				sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &t1, &t2, &t3,
															   &t4, &t5, &t6,
															   &t7, &t8, &t9);

				// push back the objects to the geometry
				obj->verts.push_back(glm::vec4(verts[t1 -1], 1.0f));
				obj->verts.push_back(glm::vec4(verts[t4 -1], 1.0f));
				obj->verts.push_back(glm::vec4(verts[t7 -1], 1.0f));

				obj->uvs.push_back(uvs[t2 -1]);
				obj->uvs.push_back(uvs[t5 -1]);
				obj->uvs.push_back(uvs[t8 -1]);

				obj->normals.push_back(glm::vec4(normals[t3-1], 0.0f));
				obj->normals.push_back(glm::vec4(normals[t6-1], 0.0f));
				obj->normals.push_back(glm::vec4(normals[t9-1], 0.0f));

				countF++;
			}

			else {
				// ignore other stuff
				continue;
			}

		}
		//cout << "VERTS: " << countVerts << " UVS: " << countUVS << " NORMALS: " << countNorms << " F's: " << countF <<  endl;

		infile.close();
	} catch (exception &e) {
		cout << e.what() << endl;
	}

}

/**
 * Display the triangles array for debugging
 */
void TextureParser::printVertices(vector<vector<glm::vec3>> triangles) {
	for (unsigned int i = 0; i < triangles.size(); i+=3) {
		cout << "index : " << i + 1 << endl;
		cout << "verts " << to_string(triangles.at(i).at(0)) << ", " << to_string(triangles.at(i).at(1)) << ", " << to_string(triangles.at(i).at(2)) << endl;
		cout << "uvs " << to_string(triangles.at(i).at(0)) << ", " << to_string(triangles.at(i).at(1)) << endl;
		cout << "normals " << to_string(triangles.at(i).at(0)) << ", " << to_string(triangles.at(i).at(1)) << ", " << to_string(triangles.at(i).at(2)) << endl << endl;
	}
	cout << "end of file" << endl;
}







