//#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 TangentLightDir;
out vec3 TangentSpotPos;
out vec3 TangentSpotDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;
uniform vec3 dirLight_dir;
uniform vec3 spotLight_pos;
uniform vec3 spotLight_dir;

void main() {
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * fragPos;
    TangentLightDir = TBN * dirLight_dir;
    TangentSpotPos = TBN * spotLight_pos;
    TangentSpotDir = TBN * spotLight_dir;

    gl_Position = projection * view * vec4(fragPos, 1.0);
}

//#shader fragment
#version 330 core
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec3 TangentLightDir;
in vec3 TangentSpotPos;
in vec3 TangentSpotDir;

uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform float material_shininess;
uniform float heightScale;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 texCoords);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords);

void main() {
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoords = ParallaxMapping(TexCoords, viewDir);

    vec3 norm = texture(texture_normal1, texCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    DirLight tangentDirLight = dirLight;
    tangentDirLight.direction = TangentLightDir;

    SpotLight tangentSpotLight = spotLight;
    tangentSpotLight.position = TangentSpotPos;
    tangentSpotLight.direction = TangentSpotDir;

    vec3 result = CalcDirLight(tangentDirLight, norm, viewDir, texCoords);
    result += CalcSpotLight(tangentSpotLight, norm, TangentFragPos, viewDir, texCoords);
    FragColor = vec4(result, 1.0);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = 1.0 - texture(texture_height1, currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = 1.0 - texture(texture_height1, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(texture_height1, prevTexCoords).r - currentLayerDepth + layerDepth;
    float weight = afterDepth / (afterDepth - beforeDepth);
    return prevTexCoords * weight + currentTexCoords * (1.0 - weight);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 texCoords) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfway = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfway), 0.0), material_shininess);
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, texCoords));
    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfway = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfway), 0.0), material_shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, texCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return ambient + diffuse + specular;
}