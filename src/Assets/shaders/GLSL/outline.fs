#version 330

in vec4 color;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

// uniform vec4 ambientLight;
// uniform vec3 lightPos;
// uniform vec4 lightColor;
uniform vec3 cameraPos;

// Hardcoded light uniforms
vec4 ambientLight = vec4(1., 1., 1., 0.2);

vec3 lightPos = vec3(-6., -2, 2);
float specularSrength = 2;
vec3 lightColor = vec3(1, 1, 1);

float diffuseIntensity = 1;
float specularIntesity = 0.2;

void main()
{
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

    vec3 diffuse;
    if (diff > 0.75) {
        diffuse = lightColor * 1;
    } else if (diff > 0.5) {
        diffuse = lightColor * 0.75;
    } else if (diff > 0.25) {
        diffuse = lightColor * 0.5;
    } else {
        diffuse = vec3(0);
    }

    // Specular
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 10);

    vec3 specular;
    if (spec > 1.0) {
        specular =  lightColor * 1;
    } else if (spec > 0.5) {
        specular =  lightColor * 0.75;
    } else if (spec > 0.25) {
        specular =  lightColor * 0.5;
    } else {
        specular = vec3(0);
    }


    // Combine the lighting and return the result
    vec3 result = (ambient + diffuse + specular) * vec3(color);
	fragColor = vec4(result, 1.0);
}
