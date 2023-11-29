#version 450

const vec2 OFFSETS[6] = vec2[](
    vec2(-1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;

layout(binding = 0) uniform CameraUBO {
    mat4 viewMatrix;
    mat4 projectionMatrix;
} camera;

layout(binding = 1) uniform GlobalLightingUBO {
    vec4 position;
    vec4 diffuse;
} glights;

const float LIGHT_RADIUS = 0.1;

void main() {
    fragOffset = OFFSETS[gl_VertexIndex];

    vec3 cameraRightWorld = {camera.viewMatrix[0][0], camera.viewMatrix[1][0], camera.viewMatrix[2][0]};
    vec3 cameraUpWorld = {camera.viewMatrix[0][1], camera.viewMatrix[1][1], camera.viewMatrix[2][1]};

    vec3 positionWorld = glights.position.xyz
    + LIGHT_RADIUS * fragOffset.x * cameraRightWorld
    + LIGHT_RADIUS * fragOffset.y * cameraUpWorld;

    gl_Position = camera.projectionMatrix * camera.viewMatrix * vec4(positionWorld, 1.0);
}