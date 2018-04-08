#version 330

layout (location = 0) in vec3 a_position;

uniform float time;

out vec2 texCoord;
out float fragTime;
out vec4 fragPos;


void main()
{
    fragTime = time;

    vec3 pos = a_position;

	texCoord = (vec2(pos) + vec2(1, 1)) / 2.0;
	
    gl_Position = vec4(pos, 1);
    fragPos = gl_Position;
}
