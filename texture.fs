#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    //phong lighting
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material_zzface;
uniform Material material_container;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient *
    //texture(material_container.diffuse, TexCoords).rgb;
    mix(texture(material_zzface.diffuse, TexCoords),texture(material_container.diffuse, TexCoords),0).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff *
    //texture(material_container.diffuse, TexCoords).rgb;
    mix(texture(material_zzface.diffuse, TexCoords),texture(material_container.diffuse, TexCoords),0).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_container.shininess);
    vec3 specular = light.specular * (spec * material_container.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}