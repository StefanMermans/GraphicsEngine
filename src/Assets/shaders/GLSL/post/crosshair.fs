#version 330

uniform sampler2D texInput0;

in float fragTime;
in vec2 texCoord;
in vec4 fragPos;

out vec4 fragColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI / 2;

void main()
{
    float normalizedSin = (sin(fragTime) + 1) / 2;

    // Basic color aquisition
    vec4 color = texture2D(texInput0, texCoord);
    
    // Basic output
    fragColor = color;

    // if (fragColor.x > normalizedSin) {
    //     discard;
    // }

    if (fragPos.x > -0.001 && fragPos.x < 0.001) {
        if (fragPos.y > -0.025 && fragPos.y < 0.025) {
            fragColor = vec4(0,1,0,1);
        }

        // if (fragPos.y >  && fragPos.y < 0.001) {
        // }
    }

    if (fragPos.y > -0.0015 && fragPos.y < 0.0015) {
        if (fragPos.x > -0.02 && fragPos.x < 0.02) {
            fragColor = vec4(0,1,0,1);
        }
    }

    // fragColor.r = fragPos.r;
    // fragColor.g = 0;
    // fragColor.b = 0;
}