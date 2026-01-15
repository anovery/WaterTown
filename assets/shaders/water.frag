#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;
in float Height;

uniform vec3 uViewPos;
uniform vec3 uWaterColor;
uniform vec3 uLightDir;
uniform float uTime;

out vec4 FragColor;

const vec3 deepWaterColor = vec3(0.0, 0.1, 0.3);
const vec3 shallowWaterColor = vec3(0.3, 0.5, 0.7);
const vec3 foamColor = vec3(0.9, 0.9, 1.0);

// 简化的菲涅尔效应
float fresnelSchlick(float cosTheta, float F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 lightDir = normalize(uLightDir);
    
    // === 1. 菲涅尔效应 ===
    float fresnel = fresnelSchlick(max(dot(norm, viewDir), 0.0), 0.02);
    
    // === 2. 漫反射光照 ===
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    
    // === 3. 镜面反射（模拟天空反射） ===
    vec3 reflectDir = reflect(-viewDir, norm);
    vec3 skyColor = mix(vec3(0.5, 0.7, 1.0), vec3(0.3, 0.5, 0.8), reflectDir.y * 0.5 + 0.5);
    vec3 reflection = skyColor * fresnel;
    
    // === 4. 折射效果（简化为深度混合） ===
    float depthFactor = clamp(Height * 2.0 + 0.5, 0.0, 1.0);
    vec3 waterColor = mix(deepWaterColor, shallowWaterColor, depthFactor);
    vec3 refraction = waterColor * (1.0 - fresnel);
    
    // === 5. 高光 ===
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = vec3(1.0) * spec * 0.5;
    
    // === 6. 泡沫效果（波峰处） ===
    float foamFactor = smoothstep(0.1, 0.15, Height);
    vec3 foam = foamColor * foamFactor * 0.3;
    
    // 最终颜色混合
    vec3 color = refraction + reflection + diffuse * 0.2 + specular + foam;
    
    // 透明度（基于菲涅尔效应）
    float alpha = mix(0.7, 0.95, fresnel);
    
    FragColor = vec4(color, alpha);
}
