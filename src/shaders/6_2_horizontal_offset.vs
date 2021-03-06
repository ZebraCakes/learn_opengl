#version 330 core
layout (location = 0) in vec3 aPos; //position has attribute position 0
layout (location = 1) in vec3 aColor; // color has attribute position 1

uniform float horizontal_offset; // horizontal offset of the triangle

out vec3 our_color; //specify a color output to the fragment shader
void main()
{
    gl_Position = vec4(aPos.x + horizontal_offset, aPos.yz, 1.0);
    our_color = aColor; // set our_color to input color from the vertex data
}