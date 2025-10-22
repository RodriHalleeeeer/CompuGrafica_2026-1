#version 330 core

#define NUMBER_OF_POINT_LIGHTS 4

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

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform int transparency;

// Prototipos
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // Propiedades
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Iluminación direccional
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // Luces puntuales
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // Spotlight
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    // Salida con alpha correcto
    vec4 albedo = texture(material.diffuse, TexCoords);
    float outAlpha = (transparency == 1) ? albedo.a : 1.0;

    if (transparency == 1 && outAlpha < 0.1)
        discard;

    color = vec4(result, outAlpha);
}

// Direccional
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Difuso
    float diff = max(dot(normal, lightDir), 0.0);

    // Especular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Texturas
    vec3 diffTex = vec3(texture(material.diffuse, TexCoords));
    vec3 specTex = vec3(texture(material.specular, TexCoords));

    // Componentes
    vec3 ambient  = light.ambient  * diffTex;
    vec3 diffuse  = light.diffuse  * diff * diffTex;
    vec3 specular = light.specular * spec * specTex;

    return ambient + diffuse + specular;
}

// Puntual
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Difuso
    float diff = max(dot(normal, lightDir), 0.0);

    // Especular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Atenuación
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance
                               + light.quadratic * (distance * distance));

    // Texturas
    vec3 diffTex = vec3(texture(material.diffuse, TexCoords));
    vec3 specTex = vec3(texture(material.specular, TexCoords));

    // Componentes
    vec3 ambient  = light.ambient  * diffTex;
    vec3 diffuse  = light.diffuse  * diff * diffTex;
    vec3 specular = light.specular * spec * specTex;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

// Spotlight
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Difuso
    float diff = max(dot(normal, lightDir), 0.0);

    // Especular (Phong)
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Atenuación
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance
                               + light.quadratic * (distance * distance));

    // Intensidad del cono
    float theta    = dot(lightDir, normalize(-light.direction));
    float epsilon  = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Texturas
    vec3 diffTex = vec3(texture(material.diffuse, TexCoords));
    vec3 specTex = vec3(texture(material.specular, TexCoords));

    // Componentes
    vec3 ambient  = light.ambient  * diffTex;
    vec3 diffuse  = light.diffuse  * diff * diffTex;
    vec3 specular = light.specular * spec * specTex;

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}
