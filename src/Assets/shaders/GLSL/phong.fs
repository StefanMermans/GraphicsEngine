#version 330

in vec4 color;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform int time;
uniform vec3 cameraPos;

// Hardcoded light uniforms
// Ambient light
vec4 ambientLight = vec4(1., 1., 1., 0.2);

// Regular light
vec3 lightPos = vec3(-6., -2, 2);
float specularSrength = 2;
vec3 lightColor = vec3(1, 1, 1);

// Regular light intensity
float diffuseIntensity = 1;
float specularIntesity = 0.2;

vec4 calcLighting() {
    // Abient lighting
    vec3 ambient = vec3(
        ambientLight.x * ambientLight.w,
        ambientLight.y * ambientLight.w,
        ambientLight.z * ambientLight.w
    );

    // Diffuse lighting
    vec3 norm = normalize(normal); // seems useless...
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(lightDir, norm), 0.);

    vec3 diffuse = diff * lightColor * diffuseIntensity;

    // Specular
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.), 10);
    vec3 specular = specularSrength * spec * lightColor * specularIntesity;

    // Combine the lighting and return the result
    vec3 result = (ambient + diffuse + specular) * vec3(color);
    return vec4(result, 1.0);
}

void main()
{
    vec4 lightColor = calcLighting();

	fragColor = lightColor;
}
