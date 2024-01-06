#version 450
#extension GL_ARB_separate_shader_objects : enable

//layout(binding = 1) uniform GlobalLightingUBO {
//    vec4 position;
//    vec4 diffuse;
//} glights;

layout(binding = 0) uniform name
{
    vec4 lightPosition[3];
    vec4 lightColor[3];
    mat4 viewMatrix;
    mat4 projectionMatrix;
	mat4 modelMatrix;
	mat4 modelNormalMatrix;
} modelUboData;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 normalInterp;
layout (location = 2) in vec3 eyeDir; 
layout (location = 3) in vec2 fragTexCoords;

layout (location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() { 
	vec4 ambientColor = vec4(255,255,255,1); // shadowColor = texture
	ambientColor = texture(texSampler,fragTexCoords);
	vec4 specularColor = vec4(0,0,0,1); // shiny part Color
	float ka = 0.1;
	float kd = 0.002;
	float ks = 0.001;
	float shininessVal = 50.0;

	vec3 N = normalize(normalInterp);

	fragColor = ka * ambientColor;
	// these will be inside the for cycle
	for (int i = 0; i < 3; i++)
	{
		vec4 diffuseColor = vec4(0,0,0,1); // materialColor
		diffuseColor = modelUboData.lightColor[i];

		vec3 L = normalize(modelUboData.lightPosition[i].xyz - vertPos);
		float lambertian = max(dot(N, L) , 0.0);
		float specular = 0.0;
		if(lambertian > 0.0)
		{
			vec3 R = reflect(-L, N);
			vec3 V = normalize(-vertPos);
			float specAngle = max(dot(R,V), 0.0);
			specular = pow(specAngle, shininessVal);
		}
		fragColor +=  kd * lambertian * diffuseColor + ks * specular * specularColor;
	}
	// end of for cycle

	//fragColor = ka * ambientColor + kd * lambertian * diffuseColor + ks * specular * specularColor;
} 

