// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program


layout(location = 0) in vec4 VertexPosition; // was vec3
layout(location = 1) in vec2 VertexUV;
layout(location = 2) in vec4 VertexNormal; // vector normal


// output to be interpolated between vertices and passed to the fragment stage
out vec2 uv;

out vec4 normalDir;
out vec3 halfAngle;
out vec3 lightDir;

// pass uniform that has the transformation matrices
uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 projection;
uniform int shadeObj;


void main()
{
	// *** shading model implementation inspiration borrowed from our course textbook, chapter 17 ***

	if (shadeObj == 1) {
	    vec4 vertPos = model * VertexPosition;
    	vec4 lightPos = vec4(0.0, 0.0, 0.0, 1.0); // light position
    
    	normalDir = model * VertexNormal;
    
    	// output to the fragment shader
    	vec3 vector = normalize(-vertPos.xyz);
    	lightDir = normalize(lightPos.xyz - vertPos.xyz);
    	halfAngle = normalize(vector + lightDir);
    
    	gl_Position = projection * view * vertPos;
	}
	else { // if sun is true
	   	gl_Position = projection * view * model * VertexPosition; // multiply by the view matrix
	}	
	
    uv = VertexUV; // assign output colour to be interpolated
      
 
}
