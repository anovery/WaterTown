#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;

// Gerstner Wave 参数
struct Wave {
    vec2 direction;
    float amplitude;
    float wavelength;
    float speed;
    float steepness;
};

uniform int uWaveCount;
uniform Wave uWaves[4];

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;
out float Height;

const float PI = 3.14159265359;

// 计算 Gerstner Wave
vec3 calculateGerstnerWave(vec3 pos) {
    vec3 result = pos;
    vec3 tangent = vec3(1.0, 0.0, 0.0);
    vec3 binormal = vec3(0.0, 0.0, 1.0);
    
    for (int i = 0; i < uWaveCount && i < 4; i++) {
        Wave wave = uWaves[i];
        
        float k = 2.0 * PI / wave.wavelength;
        float c = wave.speed;
        vec2 d = normalize(wave.direction);
        float f = k * (dot(d, pos.xz) - c * uTime);
        float a = wave.amplitude;
        float q = wave.steepness / (k * a * float(uWaveCount));
        
        // Gerstner Wave 位置偏移
        result.x += q * a * d.x * cos(f);
        result.y += a * sin(f);
        result.z += q * a * d.y * cos(f);
        
        // 计算切线和副法线（用于法线计算）
        float wa = k * a;
        float s = sin(f);
        float c_f = cos(f);
        
        tangent.x -= q * d.x * d.x * wa * s;
        tangent.y -= d.x * wa * c_f;
        tangent.z -= q * d.x * d.y * wa * s;
        
        binormal.x -= q * d.x * d.y * wa * s;
        binormal.y -= d.y * wa * c_f;
        binormal.z -= q * d.y * d.y * wa * s;
    }
    
    // 计算法线（切线 × 副法线）
    Normal = normalize(cross(binormal, tangent));
    Height = result.y;
    
    return result;
}

void main() {
    // 计算波浪变形后的位置
    vec3 worldPos = vec3(uModel * vec4(aPos, 1.0));
    vec3 displacedPos = calculateGerstnerWave(worldPos);
    
    FragPos = displacedPos;
    UV = aUV;
    
    gl_Position = uProjection * uView * vec4(displacedPos, 1.0);
}
