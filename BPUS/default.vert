#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Texture Coordinates
layout (location = 1) in vec2 aTex;

// Outputs the texture coordinates to the fragment shader
out vec2 UV;

void main() {
	// Outputs the position/coordinates of all vertices
	gl_Position = vec4(aPos, 1.0);
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	UV = aTex;
}