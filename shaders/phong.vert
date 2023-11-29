#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;
layout (location = 2) in  vec2 texCoords;

layout(binding = 0) uniform CameraUBO {
    mat4 viewMatrix;
    mat4 projectionMatrix;
} camera;

layout(binding = 1) uniform GlobalLightingUBO {
    vec4 position;
    vec4 diffuse;
} glights;

layout(binding = 3) uniform ModelUBO {
	mat4 modelMatrix;
	mat4 normalMatrix;
} model;

layout (location = 0) out vec3 vertNormal;
layout (location = 1) out vec3 lightDir;
layout (location = 2) out vec3 eyeDir;
layout (location = 3) out vec2 fragTexCoords;

void main() {
	vec4 inPos = vVertex;
	inPos.y *= -1.f;
	vec4 vertexWorldPos = model.modelMatrix * inPos;

	fragTexCoords = texCoords;
	
	mat3 normalMatrix = mat3(model.normalMatrix);

	vertNormal = normalize(normalMatrix * vNormal.xyz); /// Rotate the normal to the correct orientation 
	vec3 vertPos = vec3(camera.viewMatrix * model.modelMatrix * inPos); /// This is the position of the vertex from the origin
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;
	lightDir = normalize(glights.position.xyz - vertexWorldPos.xyz); /// Create the light direction.
	
	gl_Position =  camera.projectionMatrix * camera.viewMatrix * model.modelMatrix * inPos; 
}

