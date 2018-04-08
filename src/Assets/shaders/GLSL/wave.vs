#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int time;

out vec4 color;
out float smallTime;

void main()
{
    smallTime = time * 0.001;

	color = a_color;

    gl_Position = (view * (projection * model)) * vec4(a_position, 1.0);
    // This does the distorting
    gl_Position.x *= sin(gl_Position.x) + 1 + cos(sin(smallTime));
}