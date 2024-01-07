#version 450
#extension GL_ARB_separate_shader_objects : enable

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

const vec2 OFFSETS[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;

layout(binding = 0) uniform name
{
    vec4 position;
    mat4 viewMatrix;
    mat4 projectionMatrix;
} lightData;

const float LIGHT_RADIUS = 0.1;

void main() {
    fragOffset = OFFSETS[gl_VertexIndex];

    vec3 cameraRightWorld = {lightData.viewMatrix[0][0], lightData.viewMatrix[1][0], lightData.viewMatrix[2][0]};
    vec3 cameraUpWorld = {lightData.viewMatrix[0][1], lightData.viewMatrix[1][1], lightData.viewMatrix[2][1]};

    vec3 lightPos = lightData.position.xyz;
    lightPos.y *= -1.f;
    vec3 positionWorld = lightPos
    + LIGHT_RADIUS * fragOffset.x * cameraRightWorld
    + LIGHT_RADIUS * fragOffset.y * cameraUpWorld;

    gl_Position = lightData.projectionMatrix * lightData.viewMatrix * vec4(positionWorld, 1.0);
}