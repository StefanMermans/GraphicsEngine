#version 330

uniform sampler2D texInput0;

in float fragTime;
in vec2 texCoord;

out vec4 fragColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI / 2;

void main() {
	
	vec4 color = texture2D(texInput0, texCoord);
	
    // Simple average
    float avg = (color.r + color.g + color.b) / 3;
    fragColor = vec4(avg, avg, avg, 1);

    // Weiggted average.
    float weightedAvg = ((0.3 * color.r) + (0.59 * color.g) + (0.11 * color.b));
    fragColor = vec4(weightedAvg, weightedAvg, weightedAvg, 1);
}