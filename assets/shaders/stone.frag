#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 WorldPos;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec3 uLightColor;

out vec4 FragColor;

// 简单哈希函数
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

// 2D 噪声函数
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// 石板纹理生成
vec3 stoneTexture(vec2 worldXZ) {
    float tileScale = 2.0;
    vec2 tilePos = worldXZ * tileScale;
    vec2 grid = fract(tilePos);
    vec2 tileId = floor(tilePos);
    
    // 石板缝隙
    float gapWidth = 0.04;
    float stoneLine = step(gapWidth, grid.x) * step(gapWidth, grid.y);
    
    // 每块石板随机颜色
    float stoneNoise = hash(tileId);
    
    // 表面细节
    float surfaceDetail = noise(worldXZ * 8.0) * 0.1;
    
    // 青灰色基础
    vec3 baseColor = mix(
        vec3(0.45, 0.47, 0.50),
        vec3(0.62, 0.64, 0.68),
        stoneNoise * 0.7 + surfaceDetail
    );
    
    // 缝隙颜色
    vec3 gapColor = vec3(0.25, 0.25, 0.28);
    
    return mix(gapColor, baseColor, stoneLine);
}

void main()
{
    vec3 baseColor = stoneTexture(WorldPos.xz);
    
    // 光照计算
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * uLightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    float specularStrength = 0.2;
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;
    
    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(result, 1.0);
}
