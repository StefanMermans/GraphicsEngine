#version 330

in vec4 color;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

// uniform vec4 ambientLight;
// uniform vec3 lightPos;
uniform vec4 lightColor;

// Hardcoded light uniforms
vec3 lightPos = vec3(0, -2, 2);
vec4 ambientLight = vec4(1., 1., 1., 0.1);

void main()
{
    // Abient lighting
    vec3 ambient = vec3(
        ambientLight.x * ambientLight.w,
        ambientLight.y * ambientLight.w,
        ambientLight.z * ambientLight.w
    );
    // ambient = vec3(0);

    // Diffuse lighting
    vec3 norm = normalize(normal); // seems useless...
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.);
    vec3 diffuse = diff * vec3(1, 1, 1);

    vec3 result = (ambient + diffuse) * vec3(color);
	fragColor = vec4(result, 1.0);
}
