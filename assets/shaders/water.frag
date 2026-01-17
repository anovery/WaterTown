#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;
in float Height;

uniform vec3 uViewPos;
uniform vec3 uWaterColor;
uniform vec3 uLightDir;
uniform float uTime;

uniform int uUseBoatCutout;
uniform vec3 uBoatPos;
uniform float uBoatCutoutInner;
uniform float uBoatCutoutOuter;

// 0=circle, 1=OBB(rect) in XZ
uniform int uBoatCutoutShape;
uniform vec2 uBoatForwardXZ;
uniform vec2 uBoatHalfExtentsXZ;
uniform float uBoatCutoutFeather;

out vec4 FragColor;

const vec3 deepWaterColor = vec3(0.0, 0.1, 0.3);
const vec3 shallowWaterColor = vec3(0.3, 0.5, 0.7);
const vec3 foamColor = vec3(0.9, 0.9, 1.0);

// 简化的菲涅尔效应
float fresnelSchlick(float cosTheta, float F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    // 船只附近水面裁剪：避免水出现在船板/船舱视线里
    float cutoutMask = 1.0;
    if (uUseBoatCutout == 1) {
        if (uBoatCutoutShape == 1) {
            // OBB：用船前向定义局部坐标，做“矩形”裁剪
            vec2 delta = FragPos.xz - uBoatPos.xz;
            vec2 f = uBoatForwardXZ;
            float fl = length(f);
            if (fl < 1e-5) {
                f = vec2(0.0, 1.0);
            } else {
                f /= fl;
            }
            vec2 r = vec2(f.y, -f.x);

            // local.x = right, local.y = forward
            vec2 local = vec2(dot(delta, r), dot(delta, f));
            vec2 d = abs(local) - uBoatHalfExtentsXZ;
            float outsideDist = max(d.x, d.y);

            // outsideDist <= 0 表示在矩形内。
            // 关键：羽化只发生在“矩形内部边缘”（[-feather, 0]），矩形外部始终为 1，避免在船周围看到一块颜色不一样的矩形。
            float feather = max(uBoatCutoutFeather, 1e-5);
            cutoutMask = smoothstep(-feather, 0.0, outsideDist);
        } else {
            // Circle：旧逻辑
            float distXZ = distance(FragPos.xz, uBoatPos.xz);
            cutoutMask = smoothstep(uBoatCutoutInner, uBoatCutoutOuter, distXZ);
        }

        if (cutoutMask <= 0.001) {
            discard;
        }
    }

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
    if (uUseBoatCutout == 1) {
        alpha *= cutoutMask;
        if (alpha <= 0.01) discard;
    }
    
    FragColor = vec4(color, alpha);
}
