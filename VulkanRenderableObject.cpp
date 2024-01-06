#include "VulkanRenderableObject.h"
#include "VulkanRenderer.h"

void VulkanRenderableObject::OnCreate()
{
    printf("VulkanRenderableObjectOnCreate1\n");
    defineDescriptorSetLayoutBindingsAndDescriptorPool();
    printf("VulkanRenderableObjectOnCreate2\n");
    createDescriptorSetLayout();
    printf("VulkanRenderableObjectOnCreate3\n");
    createGraphicsPipeline();
    printf("VulkanRenderableObjectOnCreate4\n");
    createDescriptorPool();
    printf("VulkanRenderableObjectOnCreate5\n");
    createUniformBuffers();
    printf("VulkanRenderableObjectOnCreate6\n");
    createDescriptorSets();
    printf("VulkanRenderableObjectOnCreate7\n");
    updateDescriptorSets();
    printf("VulkanRenderableObjectOnCreate8\n");

    createCommandBuffer();
    printf("VulkanRenderableObjectOnCreate9\n");
}

void VulkanRenderableObject::OnDestroy()
{
    m_renderer->cleanupDescriptorSetLayout(m_descriptorSetLayout);
    m_renderer->cleanupPipeline(m_graphicsPipeline);
    m_renderer->cleanupDescriptorPool(m_descriptorPool);
    for (size_t i = 0; i < m_uniformBuffers.size(); i++) {
        m_renderer->cleanupBuffer(m_uniformBuffers[i]);
        m_renderer->cleanupMemory(m_uniformBuffersMemory[i]);
    }
    m_renderer->cleanupCommandBuffer(m_cmdBuffer);
}

void VulkanRenderableObject::Update()
{
    // set new data to be that
    // need one update uniform buffer
    // that means copy the new data to the GPU
}

void VulkanRenderableObject::defineDescriptorSetLayoutBindingsAndDescriptorPool()
{
    for (auto const& ubo: m_uboBuildData)
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = ubo.binding;
        uboLayoutBinding.descriptorCount = ubo.descriptorCount;
        uboLayoutBinding.descriptorType = ubo.descriptorType;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = ubo.shaderFlags;

        m_bindings.push_back(uboLayoutBinding);

        VkDescriptorPoolSize onePoolSize{};
        onePoolSize.type = ubo.descriptorType;
        onePoolSize.descriptorCount = ubo.poolDescriptorCount;

        m_poolSize.push_back(onePoolSize);

        m_bufferSize += m_renderer->uboOffsetPadding(ubo.bufferRange);
    }
}

void VulkanRenderableObject::addUniformBufferBuildData(const uboBuildData& newData)
{
    m_uboBuildData.push_back(newData);
}

void VulkanRenderableObject::addUniformBufferBuildData(uint32_t &binding, uint32_t &descriptorCount, VkDescriptorType &descriptorType, VkShaderStageFlags &shaderFlags, VkDeviceSize &bufferOffset, VkDeviceSize &bufferRange, uint32_t &poolDescriptorCount)
{
    uboBuildData uboData;
    uboData.binding = binding;
    uboData.descriptorCount = descriptorCount;
    uboData.descriptorType = descriptorType;
    uboData.shaderFlags = shaderFlags;
    uboData.bufferOffset = bufferOffset;
    uboData.bufferRange = bufferRange;
    uboData.poolDescriptorCount = poolDescriptorCount;
    addUniformBufferBuildData(uboData);
}

void VulkanRenderableObject::createCommandBuffer()
{
    m_renderer->createSecondaryCommandBuffersStart(m_cmdBuffer);

    for (int i = 0; i < m_cmdBuffer.size(); i++) {
        updateCommandBuffer(m_cmdBuffer[i], i);
    }

    m_renderer->createSecondaryCommandBuffersFinish(m_cmdBuffer);

    m_renderer->addCommandToCommandBuffer(m_cmdBuffer);
}

void VulkanRenderableObject::createGraphicsPipeline()
{
    m_renderer->CreateGraphicsPipeline(m_vertFilePath, m_fragFilePath , m_descriptorSetLayout, m_pipelineLayout, m_graphicsPipeline, m_isVertexed, m_isPushConstantUsed, m_withGeometryShader, "");
}

void VulkanRenderableObject::createDescriptorSetLayout()
{
    m_renderer->createDescriptorSetLayout(m_descriptorSetLayout, m_bindings);
}

void VulkanRenderableObject::createDescriptorPool()
{
    m_renderer->createDescriptorPool(m_descriptorPool, m_poolSize);
}

void VulkanRenderableObject::createDescriptorSets()
{
    m_renderer->createDescriptorSets(m_descriptorSetLayout, m_descriptorPool, m_descriptorSets);
}

void VulkanRenderableObject::createUniformBuffers()
{
    std::cout << "buffersize " << m_bufferSize << std::endl;
    m_renderer->createUniformBuffers(m_bufferSize, m_uniformBuffers, m_uniformBuffersMemory, m_uniformBufferMemoryMap);
}

void VulkanRenderableObject::updateDescriptorSets()
{
    printf("VulkanRenderableObjectupdateDescriptorSets1\n");

    printf("VulkanRenderableObjectupdateDescriptorSets2\n");
    for (size_t i = 0; i < m_uniformBuffers.size(); i++) {
        std::vector<VkWriteDescriptorSet> oneDescriptorWrites{};
        oneDescriptorWrites.resize(m_uboBuildData.size());
        printf("VulkanRenderableObjectupdateDescriptorSets3\n");

        for(size_t j = 0; j < m_uboBuildData.size(); j++)
        {
            VkWriteDescriptorSet writeSet{};
            printf("VulkanRenderableObjectupdateDescriptorSets4\n");
            writeSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            printf("VulkanRenderableObjectupdateDescriptorSets4.1\n");
            writeSet.dstSet = m_descriptorSets[i];
            printf("VulkanRenderableObjectupdateDescriptorSets4.2\n");
            writeSet.dstBinding = m_uboBuildData[j].binding;
            printf("VulkanRenderableObjectupdateDescriptorSets4.3\n");
            writeSet.dstArrayElement = 0;
            printf("VulkanRenderableObjectupdateDescriptorSets4.4\n");
            writeSet.descriptorType = m_uboBuildData[j].descriptorType;
            printf("VulkanRenderableObjectupdateDescriptorSets4.5\n");
            writeSet.descriptorCount = m_uboBuildData[j].descriptorCount;
            printf("VulkanRenderableObjectupdateDescriptorSets4.6\n");
            if(m_uboBuildData[j].descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            {
                VkDescriptorImageInfo imageInfo{};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = m_textureView;
                imageInfo.sampler = m_renderer->getImageSampler();
                writeSet.pImageInfo = &imageInfo;
                printf("VulkanRenderableObjectupdateDescriptorSets5\n");
            }
            else
            {
                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = m_uniformBuffers[i];
                bufferInfo.offset = m_uboBuildData[j].bufferOffset;
                std::cout << "descriptorset update offset " << m_uboBuildData[j].bufferOffset << std::endl;
                bufferInfo.range = m_uboBuildData[j].bufferRange;
                writeSet.pBufferInfo = &bufferInfo;
                printf("VulkanRenderableObjectupdateDescriptorSets6\n");
            }
            oneDescriptorWrites[j] = writeSet;
        }
        printf("VulkanRenderableObjectupdateDescriptorSets7\n");
        printf("VulkanRenderableObjectupdateDescriptorSets8\n");
        m_renderer->updateDescriptorSets(oneDescriptorWrites);
    }
    printf("VulkanRenderableObjectupdateDescriptorSets9\n");
    printf("VulkanRenderableObjectupdateDescriptorSets10\n");
}
