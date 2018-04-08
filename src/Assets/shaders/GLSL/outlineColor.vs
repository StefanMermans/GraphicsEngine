#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelViewProjectionMatrix;

out vec4 color;

void main()
{
	color = vec4(1, 0, 0, 1);

    // gl_Position = (model * view * projection) * vec4(a_position, 1.0);
    gl_Position = (view * (projection * model)) * vec4(a_position, 1.0);
}