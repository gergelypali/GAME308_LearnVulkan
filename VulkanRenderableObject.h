#ifndef VULKAN_RENDERABLE_OBJECT_H
#define VULKAN_RENDERABLE_OBJECT_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <utility>
#include "Structs.h"

class VulkanRenderer;

class VulkanRenderableObject
{
public:
	// constructor
	VulkanRenderableObject(
		VulkanRenderer* renderer,
		const std::string& vertFilePath,
		const std::string& fragFilePath
		) :
		m_renderer(renderer),
		m_vertFilePath(vertFilePath),
		m_fragFilePath(fragFilePath)
		{};
	void OnCreate();
	void OnDestroy();
	void Update();

	void addUniformBufferBuildData(const uboBuildData& newData);
	void addUniformBufferBuildData(
		uint32_t& binding,
		uint32_t& descriptorCount,
		VkDescriptorType& descriptorType,
		VkShaderStageFlags& shaderFlags,
		VkDeviceSize& bufferOffset,
		VkDeviceSize& bufferRange,
		uint32_t& poolDescriptorCount
	);

	void virtual updateCommandBuffer(VkCommandBuffer& buffer, int& idx) = 0;
	void createCommandBuffer();
	void drawRectangle(VkCommandBuffer& buffer);
	inline VulkanRenderer* getRenderer() { return m_renderer; };

protected:
	bool m_isVertexed{false};
	bool m_isPushConstantUsed{false};
	bool m_withGeometryShader{false};
	//variables for vulkan rendering
    VulkanRenderer* m_renderer{nullptr};

	VkPipeline m_graphicsPipeline;
	VkPipelineLayout m_pipelineLayout;
	VkDescriptorSetLayout m_descriptorSetLayout;
	VkDescriptorPool m_descriptorPool;
	std::vector<VkDescriptorSet> m_descriptorSets;
    std::vector<VkBuffer> m_uniformBuffers;
    std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*> m_uniformBufferMemoryMap;
	std::vector<void*> m_perUniformBufferMemoryMap;
	VkDeviceSize m_bufferSize{0};

	std::vector<VkCommandBuffer> m_cmdBuffer;

	std::vector<uboBuildData> m_uboBuildData;

    const std::string m_vertFilePath{};
	const std::string m_fragFilePath{};

	// texture part
    VkImage m_textureImage;
    VkDeviceMemory m_textureImageMemory;
    VkImageView m_textureView;

	std::vector<VkDescriptorSetLayoutBinding> m_bindings;
	std::vector<VkDescriptorPoolSize> m_poolSize;

	// vulkan renderer calls
	void defineDescriptorSetLayoutBindingsAndDescriptorPool();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createDescriptorPool();
	void createUniformBuffers();
	void createDescriptorSets();
	void updateDescriptorSets();
};

#endif // VULKAN_RENDERABLE_OBJECT_H