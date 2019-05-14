// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec2 uv;

in vec4 normalDir;
in vec3 halfAngle;
in vec3 lightDir;


// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

uniform sampler2D imageTexture; // image colour loaded here


uniform vec3 ka, kd, ks;
uniform float phongExponent;
uniform int shade;

void main(void)
{
	// *** shading model implementation inspiration borrowed from our course textbook, chapter 17 ***

	if (shade == 1) {
		vec3 n = normalize(normalDir.xyz);
		vec3 h = normalize(halfAngle);
		vec3 l = normalize(lightDir);
		
		vec4 objColour = texture(imageTexture, uv);
		
		vec3 intensity = ka * vec3(1.0, 1.0, 1.0) + 
						 kd * objColour.rgb * max(0.0, dot(n, l)) + 
						 ks * objColour.rgb * pow(max(0.0, dot(n, h)), phongExponent);
						 
		FragmentColour.rgb = intensity;
		FragmentColour.a = objColour.a;
	}
	else { // if the sun is the object being checked
		FragmentColour = texture(imageTexture, uv);
	}
	
}
