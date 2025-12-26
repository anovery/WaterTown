#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    // 计算世界空间中的片段位置
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    
    // 将法线变换到世界空间（使用法线矩阵避免非均匀缩放问题）
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    
    // 最终顶点位置
    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}
