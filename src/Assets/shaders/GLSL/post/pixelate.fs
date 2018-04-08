#version 330

uniform sampler2D texInput0;

in float fragTime;
in vec2 texCoord;

out vec4 fragColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI / 2;

float vx_offset = 2;
float screenWidth = 2048.0; 
float screenHeight = 2048.0; 
float pixelWidth = 10.;
float pixelHeight = 10.;

vec4 pixelate() {
    vec4 tc = vec4(1.0, 0.0, 0.0, 1.0);
    if (texCoord.x < (vx_offset - 0.005))
    {
        float dx = pixelWidth * (1.0 / screenWidth);
        float dy = pixelHeight * (1.0 / screenHeight);
        vec2 coord = vec2(
            dx * floor(texCoord.x / dx),
            dy * floor(texCoord.y / dy)
        );
        tc = texture2D(texInput0, coord);
    }
    else if (texCoord.x >= (vx_offset + 0.005))
    {
        tc = texture2D(texInput0, texCoord);
    }
    return tc;
}

void main()
{
    fragColor = pixelate();
}
