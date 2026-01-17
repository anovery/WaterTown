#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 VertexColor;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform bool uUseVertexColor;

out vec4 FragColor;

void main()
{
    // 环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;
    
    // 漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    // 镜面反射光照
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;
    
    vec3 baseColor = uUseVertexColor ? VertexColor : uObjectColor;
    // 最终颜色
    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(result, 1.0);
}
