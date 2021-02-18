#version 330 core
layout (location = 0) in vec3 aPos; //position has attribute position 0

out vec3 our_pos; //specify a color output to the fragment shader
void main()
{
    gl_Position = vec4(aPos, 1.0);

    //NOTE(amp): color values < 0 are clamped to 0, thus the bottom left corner
    //           of the tri is black
    our_pos= aPos; // set our_color to input color from the vertex data
}