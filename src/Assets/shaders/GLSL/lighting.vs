#version 330

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_texCoord;
layout (location = 3) in vec3 a_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
out vec3 normal;
out vec3 fragPos;

void main()
{   
	color = a_color;
    normal = a_normal;
    fragPos = vec3(model * vec4(a_position, 1.0));

    color = vec4(0.5, 0.5, 0.5, 1.);
    gl_Position = (view * (projection * model)) * vec4(a_position, 1.0);
}