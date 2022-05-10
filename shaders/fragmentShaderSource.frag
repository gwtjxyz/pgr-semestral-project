#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight {
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

#define NR_MATERIALS 3

// TODO handle multiple materials properly
uniform Material materials[NR_MATERIALS];
uniform DirLight dirLight;
uniform Spotlight spotlight;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
in vec3 viewPos;
uniform vec3 fogCenter;

out vec4 FragColor;

vec3 CombineLight(vec3 ambient, vec3 diffuse, vec3 specular, float diff, float spec);

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalcFogColor(vec3 shadedColor);

void main() {
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // point lights
    for (int i = 0; i != NR_POINT_LIGHTS; ++i) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    // spot light
    result += CalcSpotLight(spotlight, norm, FragPos, viewDir);

    vec3 resultWithFog = CalcFogColor(result);

    FragColor = vec4(resultWithFog, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materials[0].shininess);
    // combine results
    vec3 result = CombineLight(light.ambient, light.diffuse, light.specular, diff, spec);
    return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materials[0].shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    // combine results
    vec3 result = CombineLight(light.ambient, light.diffuse, light.specular, diff, spec);
    result *= attenuation;
    return result;
}

vec3 CalcSpotLight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materials[0].shininess);
    // combine results
    vec3 result = CombineLight(light.ambient, light.diffuse, light.specular, diff, spec);
    result *= intensity;
    return result;
}

vec3 CombineLight(vec3 ambient, vec3 diffuse, vec3 specular, float diff, float spec) {
    vec3 newAmbient  = ambient  * vec3(texture(materials[0].diffuse, TexCoords));
    vec3 newDiffuse  = diffuse  * diff * vec3(texture(materials[0].diffuse, TexCoords));
    vec3 newSpecular = specular * spec * vec3(texture(materials[0].specular, TexCoords));
    return newAmbient + newDiffuse + newSpecular;
}

// TODO make this take in uniforms
vec3 CalcFogColor(vec3 shadedColor) {
    // parameters
    float fogMaxDist = 80.0;
    float fogMinDist = 20.0;
//    float fogColor = max(max(dirLight.ambient.x, dirLight.ambient.y), dirLight.ambient.z);

    vec3 fogColorVec = vec3(0.4, 0.4, 0.45);

    // calculate fog
    float dist = length(fogCenter - FragPos);
    float fogFactor = (fogMaxDist - dist) / (fogMaxDist - fogMinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    vec3 outputColor = mix(fogColorVec, shadedColor, fogFactor);
    return outputColor;
}