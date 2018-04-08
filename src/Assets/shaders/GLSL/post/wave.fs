#version 330

uniform sampler2D texInput0;

in float fragTime;
in vec2 texCoord;

out vec4 fragColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI / 2;

const float offset = 0.01;

void main()
{
    float modifier = (sin(fragTime * (texCoord.y * sin(fragTime) * 0.3)) + 1) / 2;
    vec2 uv = texCoord.xy;
    uv.x += modifier * offset;

    // Basic color aquisition
    vec4 color = texture2D(texInput0, uv );
    
    // Basic output
    fragColor = color;
}