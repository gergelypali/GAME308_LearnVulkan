#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "Vector.h"

class PointLight
{
private:
	const std::string m_vertFilePath{"shaders/lightPoint_vert.spv"};
	const std::string m_fragFilePath{"shaders/lightPoint_frag.spv"};

	// local
	VkPipeline m_graphicsPipeline;
	VkPipelineLayout m_pipelineLayout;

	// from device/engine
	VkDevice m_logicalDevice;
	VkRenderPass m_renderPass;
	VkDescriptorSetLayout m_descriptorSetLayout;

	// helpers
	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createGraphicsPipeline();

public:
	PointLight(VkDevice device, VkRenderPass renderpass, VkDescriptorSetLayout layout);

	void Update(float deltaTime);
	void Render() {};

	void UpdateCommandBuffer(VkCommandBuffer buffer, VkDescriptorSet set);

	// getters
	inline VkPipeline getPipeline() { return m_graphicsPipeline; };
};

#endif // !POINT_LIGHT_H
