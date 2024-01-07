#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in  vec3 vVertex;
layout (location = 1) in  vec3 vNormal;
layout (location = 2) in  vec2 texCoords;

layout(binding = 0) uniform name
{
    vec4 lightPosition[3];
    vec4 lightColor[3];
    mat4 viewMatrix;
    mat4 projectionMatrix;
	mat4 modelMatrix;
	mat4 modelNormalMatrix;
} modelUboData;

layout(binding = 1) uniform sampler2D texSampler;

layout (location = 0) out vec3 vertPos;
layout (location = 1) out vec3 normalInterp;
layout (location = 2) out vec3 eyeDir;
layout (location = 3) out vec2 fragTexCoords;

void main() {
	fragTexCoords = texCoords;
	vec4 vertexWorldPos = modelUboData.modelMatrix * vec4(vVertex, 1.0);
	vec4 vertPos4 = modelUboData.viewMatrix * vertexWorldPos;
  	vertPos = vec3(vertexWorldPos) / vertPos4.w;
	normalInterp = vec3(modelUboData.modelNormalMatrix * vec4(vNormal, 0.0));
	gl_Position = modelUboData.projectionMatrix * vertPos4;
	gl_Position.y *= -1.f;
}
