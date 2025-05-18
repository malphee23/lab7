#version 410 core
out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

struct lightStruct {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct materialStruct {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform lightStruct light_1;
uniform materialStruct mat_1;

void main() {
    // ambient
    vec3 ambient = light_1.ambient * mat_1.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light_1.position - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light_1.diffuse * (diff * mat_1.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat_1.shininess);
    vec3 specular = light_1.specular * (spec * mat_1.specular);

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
