#version 330 core

// Outputs colors in RGBA
out vec4 color;

// Inputs the texture coordinates from the Vertex Shader
in vec2 UV;

// Gets the texture Unit from the main function
uniform sampler2D tex0;

// Uniform for color or texture
uniform int usetex = 1;

// Color drawn when usetex == 0
uniform vec4 debug_color;

void main() {
    if (usetex == 1)
        color = texture(tex0, UV);
    else
        color = debug_color;
}