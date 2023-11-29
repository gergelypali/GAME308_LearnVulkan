#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 lightDir;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 kTexture = texture(texSampler, fragTexCoord);
    vec4 ka = 0.01 * kTexture;
    outColor = ka;

    outColor += kTexture * vec4(0,1,1,0) * 0.5 * vec4(0.6,0.6,0.6,0) + vec4(0,1,1,0) * vec4(0.6,0.6,0.6,0);

    /// outColor = texture(texSampler, fragTexCoord);
}