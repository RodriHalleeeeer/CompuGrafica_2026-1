#version 330 core

struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;   // Sol
uniform Light light2;  // Luna
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

vec3 phongPoint(Light L, vec3 N, vec3 V, vec3 P)
{
    vec3 Ldir = normalize(L.position - P);
    // Difuso
    float diff = max(dot(N, Ldir), 0.0);
    // Especular (Blinn-Phong)
    vec3 H = normalize(Ldir + V);
    float spec = pow(max(dot(N, H), 0.0), material.shininess);

    vec3 ambient  = L.ambient  * material.ambient;
    vec3 diffuse  = L.diffuse  * material.diffuse * diff;
    vec3 specular = L.specular * material.specular * spec;
    return ambient + diffuse + specular;
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 c1 = phongPoint(light,  N, V, FragPos);
    vec3 c2 = phongPoint(light2, N, V, FragPos);

    vec3 color = c1 + c2;
    FragColor = vec4(color, 1.0);
}
