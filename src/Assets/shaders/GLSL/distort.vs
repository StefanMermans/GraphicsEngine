#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;

void main()
{
	color = a_color;

    gl_Position = model * view * projection * vec4(a_position, 1.);
    // This does the distorting
    // gl_Position.x *= sin(gl_Position.x) + 1;
}