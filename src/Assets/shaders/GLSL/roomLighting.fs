#version 330

in vec4 color;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texInput0;
uniform sampler2D texInput1;

// uniform vec4 ambientLight;
// uniform vec3 lightPos;
// uniform vec4 lightColor;
uniform int time;
uniform vec3 cameraPos;

// Hardcoded light uniforms
vec4 ambientLight = vec4(1., 1., 1., 0.2);

vec3 lightPos = vec3(-6., -2, 2);
float specularSrength = 2;
vec3 lightColor = vec3(1, 1, 1);

float diffuseIntensity = 0.5;
float specularIntesity = 0.5;

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

vec4 calcTextures() {
    vec4 color0 = texture(texInput0, texCoord);

    vec4 color1 = texture(texInput1, texCoord);
   
    // Render only the color of texture 0 if the other texture's
    // color is transparent.
    if (color1.w == 0) {
        return color0;
    }

    return mix(color0, color1, (sin(time * 0.001) + 1) / 2);
}

void main()
{
    vec4 lightColor = calcLighting();
    vec4 texColor = calcTextures();

	fragColor = lightColor * texColor;
    }
