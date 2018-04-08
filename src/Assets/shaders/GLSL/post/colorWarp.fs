#version 330

uniform sampler2D texInput0;

in float fragTime;
in vec2 texCoord;

out vec4 fragColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI / 2;

void main()
{
    // Plotted curves: https://www.desmos.com/calculator/kfrhliexch
    float redCurve = (sin(fragTime) + 1) / 2;
    float greenCurve = (sin(fragTime + PI_2) + 1) / 2;
    float blueCurve = (sin(fragTime - PI_2) + 1) / 2;

    // Basic color aquisition
    vec4 color = texture2D(texInput0, texCoord);

    // Crazy inverted color overflow
    color.r = (redCurve - color.r);    
    color.g = (greenCurve - color.g);
    color.b = (blueCurve - color.b);

    // Boring color overflow 
    // color.r = (redCurve * color.r);    
    // color.g = (greenCurve * color.g);
    // color.b = (blueCurve * color.b);

    fragColor = color;    
}