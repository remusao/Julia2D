#version 150 core

uniform float dx;
uniform float dy;

in vec2 position;
out vec2 pixPos;


void main()
{
    gl_Position = vec4(position, 1.0, 1.0);
    pixPos = vec2(gl_Position.x + dx, gl_Position.y + dy);
}
