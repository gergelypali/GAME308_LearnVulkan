#include "PointLight.h"
#include "VulkanRenderer.h"

using namespace MATH;

void PointLight::OnCreate()
{
    printf("PointLightOnCreate1\n");

    uboBuildData first;
    first.binding = 0;
    first.descriptorCount = 1;
    first.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    first.shaderFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    first.bufferOffset = 0;
    first.bufferRange = sizeof(pointLightUboData);
    first.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    //addUniformBufferBuildData(first);

    printf("PointLightOnCreate2\n");
    uboBuildData second;
    second.binding = 1;
    second.descriptorCount = 1;
    second.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    second.shaderFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    second.bufferOffset = m_renderer->uboOffsetPadding(sizeof(pointLightUboData));
    second.bufferRange = sizeof(CameraUBO);
    second.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    //addUniformBufferBuildData(second);
    printf("PointLightOnCreate3\n");

    uboBuildData third;
    third.binding = 0;
    third.descriptorCount = 1;
    third.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    third.shaderFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    third.bufferOffset = 0;
    third.bufferRange = sizeof(pointLightUboData);
    third.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    addUniformBufferBuildData(third);

    m_isPushConstantUsed = true;

    VulkanRenderableObject::OnCreate();
    printf("PointLightOnCreate4\n");
}

void PointLight::Update(const MATH::Vec4& newPos, const MATH::Matrix4& view, const MATH::Matrix4& projection)
{
    lightUbo.position = newPos;
    lightUbo.cameraProjection = projection;
    lightUbo.cameraView = view;
    /*
    std::cout << "viewbol posX " << view[0] << std::endl;
    std::cout << "viewbol posY " << view[1] << std::endl;
    std::cout << "viewbol posZ " << view[2] << std::endl;
    std::cout << "viewbol colorX " << view[4] << std::endl;
    std::cout << "viewbol colorY " << view[5] << std::endl;
    std::cout << "viewbol colorZ " << view[6] << std::endl;
    */
    m_renderer->updateUniformBuffer(
        &lightUbo,
        m_uniformBufferMemoryMap,
        m_uboBuildData[0].bufferOffset,
        m_uboBuildData[0].bufferRange
    );

    /*
    m_renderer->updateUniformBuffer(
        &lightUbo,
        m_uniformBufferMemoryMap,
        m_uboBuildData[0].bufferOffset,
        m_uboBuildData[0].bufferRange
    );
    m_renderer->updateUniformBuffer(
        &cameraUbo,
        m_uniformBufferMemoryMap,
        m_uboBuildData[1].bufferOffset,
        m_uboBuildData[1].bufferRange
    );
    */
}

void PointLight::updateCommandBuffer(VkCommandBuffer& buffer, int& idx)
{
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
    vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSets[idx], 0, nullptr);
    vkCmdPushConstants(buffer, m_pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(lightPushC), &lightConstant);
    vkCmdDraw(buffer, 6, 1, 0, 0);
}
