#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragOffset;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform name
{
    vec4 position;
    mat4 viewMatrix;
    mat4 projectionMatrix;
} lightData;

layout(push_constant) uniform constants
{
	vec4 color;
} PushConstants;

void main() {
    float dis = sqrt(dot(fragOffset, fragOffset));
    if (dis >= 1.0)
    {
        discard;
    }
    outColor = PushConstants.color;
}