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

// FBM 分形布朗运动
float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    
    for (int i = 0; i < 4; i++) {
        value += amplitude * noise(p * frequency);
        frequency *= 2.0;
        amplitude *= 0.5;
    }
    
    return value;
}

// 草地纹理生成
vec3 grassTexture(vec2 worldXZ) {
    float noise1 = fbm(worldXZ * 3.0);
    float noise2 = fbm(worldXZ * 7.0 + vec2(100.0));
    
    vec3 grassDark = vec3(0.18, 0.42, 0.12);
    vec3 grassMid = vec3(0.30, 0.58, 0.22);
    vec3 grassLight = vec3(0.45, 0.72, 0.32);
    
    vec3 baseGrass = mix(grassDark, grassMid, noise1);
    baseGrass = mix(baseGrass, grassLight, noise2 * 0.4);
    
    float detail = noise(worldXZ * 15.0) * 0.08;
    baseGrass += vec3(detail * 0.5, detail, detail * 0.3);
    
    return baseGrass;
}

void main()
{
    vec3 baseColor = grassTexture(WorldPos.xz);
    
    // 光照计算
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * uLightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    float specularStrength = 0.1;
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * uLightColor;
    
    vec3 result = (ambient + diffuse + specular) * baseColor;
    FragColor = vec4(result, 1.0);
}
