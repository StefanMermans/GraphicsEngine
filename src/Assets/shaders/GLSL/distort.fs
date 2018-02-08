#version 330

in vec4 color;
out vec4 fragColor;

void main()
{
    vec4 modifier = vec4(1, 1, 40, 1);
    fragColor = color * modifier;
}
