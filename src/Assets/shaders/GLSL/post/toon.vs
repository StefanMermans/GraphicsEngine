#version 330

layout (location = 0) in vec3 a_position;

out vec2 texCoord;
out float fragTime;

uniform float time;

void main()
{
    fragTime = time;

    vec3 pos = a_position;

	texCoord = (vec2(pos) + vec2(1, 1)) / 2.0;
	gl_Position = vec4(pos, 1);
}
