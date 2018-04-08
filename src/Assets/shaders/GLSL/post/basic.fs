#version 330

uniform sampler2D texInput0;

in float fragTime;
in vec2 texCoord;

out vec4 fragColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI / 2;

void main()
{
    // Basic color aquisition
    vec4 color = texture2D(texInput0, texCoord);
    
    // Basic output
    fragColor = color;
}