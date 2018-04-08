#version 330

in vec4 color;
out vec4 fragColor;
in float smallTime;

void main()
{
    vec4 modifier = vec4(sin(smallTime), tan(smallTime), cos(smallTime), 1);

    fragColor = color;
    fragColor = modifier;
}
