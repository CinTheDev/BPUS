#version 330 core

// Outputs colors in RGBA
out vec4 color;

// Inputs the texture coordinates from the Vertex Shader
in vec2 UV;

// Gets the texture Unit from the main function
uniform sampler2D tex0;

void main() {
    color = texture(tex0, UV);
}