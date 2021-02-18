#version 330 core
out vec4 FragColor;

in vec3 our_pos;

void main()
{
    FragColor = vec4(our_pos, 1.0);
}