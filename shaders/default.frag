#version 330 core

const int numPLights = 2;
const int numDLights = 1;

in VertexOut {

    vec2 texCoords;
    vec3 normal;
    
    vec3 fragPos;
    vec3 tFragPos;
    vec3 tViewPos;

    mat3 tbn;
} vs_in;

struct pointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
    float cutoff;
};
uniform pointLight pointLights[numPLights];
in vec3 tPLightPos[numPLights];

struct directionalLight {
    vec3 position;
    vec3 color;
    float intensity;
};
uniform directionalLight directionalLights[numDLights];
in vec3 tDLightPos[numDLights];
in vec3 tDLightDir[numDLights];

uniform int Time;
uniform vec2 Resolution;

uniform int use_diffuseMap;
uniform int use_specularMap;
uniform int use_normalMap;
uniform int use_heightMap;
uniform int use_emissiveMap;
uniform int use_alphaMap;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;
uniform sampler2D emissiveMap;
uniform sampler2D alphaMap;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 CameraPosition;

out vec4 fragColor;


vec3 diffuse(vec3 light_dir, vec3 normal, vec2 texCoords)
{
    return max(dot(normal, light_dir), 0.0f) * texture(diffuseMap, texCoords).rgb;
}

vec3 specular(vec3 light_dir, vec3 normal, vec3 view_dir, vec2 texCoords) 
{
    vec3 specularity = vec3(0.1f);
    float shininess = 25.6f;  // TODO: Get this from material
    if(use_specularMap == 1) {
       specularity = texture(specularMap, texCoords).rgb;
    }
    
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_power = pow(max(dot(view_dir, reflect_dir), 0.0f), shininess);

    return specularity * specular_power;
}

vec3 addDirectionalLight(int index, vec2 texCoords, vec3 lightDir, vec3 viewDir, vec3 normal)
{
    directionalLight light = directionalLights[index];

    // TODO: implement light specular color and use that instead of light.color in specular
    vec3 diff = light.color * diffuse(lightDir, normal, texCoords);
    vec3 spec = light.color * specular(lightDir, normal, viewDir, texCoords);
    vec3 value = diff + spec;

    return value * light.intensity;
}

vec3 addPointLight(int index, vec2 texCoords, vec3 lightDir, vec3 viewDir, vec3 normal)
{
    pointLight light = pointLights[index];

    // Attenuation
    float d = distance(tPLightPos[index], vs_in.tFragPos);
    float dr = (max(d - light.radius, 0.0f) / light.radius) + 1.0f;
    float attenuation = 1.0f / (dr * dr);
    attenuation = (attenuation - light.cutoff) / (1.0f - light.cutoff);
    attenuation = max(attenuation, 0.0f);

    // TODO: implement light specular color and use that instead of light.color in specular
    vec3 diff = light.color * diffuse(lightDir, normal, texCoords);
    vec3 spec = light.color * specular(lightDir, normal, viewDir, texCoords);
    vec3 value = diff + spec;

    return value * attenuation;
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir, float scale, out float parallaxHeight) 
{
    const float minLayers = 10.0f;
    const float maxLayers = 20.0f;
    float layers = 20.0f; //mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDir)));

    float depth = 1.0f / layers;
    float currentDepth = 0.0f;

    vec2 offset = texCoords;
    float heightValue = texture(diffuseMap, offset).r;

    vec2 delta = scale * viewDir.xy / -viewDir.z / layers;

    while(heightValue > currentDepth)
    {
        currentDepth += depth;
        offset -= delta;
        heightValue = texture(diffuseMap, offset).r;
    }
    
    vec2 prevTexCoords = offset + delta;
    float nextHeight = heightValue - currentDepth;
    float prevHeight = texture(diffuseMap, prevTexCoords).r - currentDepth + depth;
    float weight = nextHeight / (nextHeight - prevHeight);
    vec2 finalTexCoords = prevTexCoords * weight + offset * (1.0f - weight);

    parallaxHeight = currentDepth + prevHeight * weight + nextHeight * (1.0f - weight);

    return finalTexCoords;
}

vec2 par(vec2 texCoords, vec3 viewDir)

{ 
    float height = texture(heightMap, vs_in.texCoords).r;
    float p = height * 0.04f;
    return texCoords + viewDir.xy * p;    
}

void main(void)
{
    vec3 viewDir = normalize(vs_in.tViewPos - vs_in.tFragPos);
    vec2 texCoords = vs_in.texCoords;


    //////////////////////
    //   Texture Maps   //
    //////////////////////

    vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
    vec3 normal = normalize(vs_in.normal);

    if(use_alphaMap == 1)
    {
        float value = texture(alphaMap, texCoords).r;
        if(value < 0.1f)
        {
            discard;
        }
    }
    
    if(use_diffuseMap == 1)
    {
        ambient *= texture(diffuseMap, texCoords).rgb;
    }
    
    if(use_normalMap == 1)
    {
        normal = normalize(texture(normalMap, vs_in.texCoords).rgb * 2.0f - 1.0f);
    }


    //////////////////////
    // Calculate lights //
    //////////////////////

    vec3 light_accumulator = vec3(0.0f);

    // Directional lights
    for(int i = 0; i < numDLights; i++)
    {
        vec3 lightDir = normalize(tDLightPos[i]);
        light_accumulator += vec3(addDirectionalLight(i, texCoords, lightDir, viewDir, normal));
    }
    
    // Point lights
    for(int i = 0; i < numPLights; i++)
    {
        vec3 lightDir = normalize(tPLightPos[i] - vs_in.tFragPos);
        light_accumulator += vec3(addPointLight(i, texCoords, lightDir, viewDir, normal)) * pointLights[i].intensity;
    }

    vec3 color = ambient + light_accumulator;
    fragColor = vec4(color, 1.0f);
}
