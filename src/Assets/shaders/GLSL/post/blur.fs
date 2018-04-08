#version 330

uniform sampler2D texInput0;

in float fragTime;
in vec2 texCoord;

out vec4 fragColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI / 2;

vec2 delta = vec2(3, 3);
vec2 resolution = vec2(2048, 2048);

void main() {

	vec4 sum = vec4( 0. );
	vec2 inc = delta / resolution;

	sum += texture2D( texInput0, ( texCoord - inc * 4. ) ) * 0.051;
	sum += texture2D( texInput0, ( texCoord - inc * 3. ) ) * 0.0918;
	sum += texture2D( texInput0, ( texCoord - inc * 2. ) ) * 0.12245;
	sum += texture2D( texInput0, ( texCoord - inc * 1. ) ) * 0.1531;
	sum += texture2D( texInput0, ( texCoord + inc * 0. ) ) * 0.1633;
	sum += texture2D( texInput0, ( texCoord + inc * 1. ) ) * 0.1531;
	sum += texture2D( texInput0, ( texCoord + inc * 2. ) ) * 0.12245;
	sum += texture2D( texInput0, ( texCoord + inc * 3. ) ) * 0.0918;
	sum += texture2D( texInput0, ( texCoord + inc * 4. ) ) * 0.051;

	fragColor = sum;
}