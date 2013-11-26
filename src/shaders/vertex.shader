#version 150 core

uniform mat4 mvp;

in vec2 position;
out vec4 pixPos;


void main()
{
    gl_Position = vec4(position, 1.0, 1.0);
    pixPos = mvp * gl_Position;
}
