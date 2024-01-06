#include "Model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "VulkanRenderer.h"

void Model::LoadModelIndexed() {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, m_fileName)) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
                m_vertices.push_back(vertex);
            }

            m_indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void Model::OnCreate()
{
    uboBuildData first;
    first.binding = 0;
    first.descriptorCount = 1;
    first.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    first.shaderFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_GEOMETRY_BIT;
    first.bufferOffset = 0;
    first.bufferRange = sizeof(modelUboData);
    first.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    addUniformBufferBuildData(first);

    uboBuildData sampler;
    sampler.binding = 1;
    sampler.descriptorCount = 1;
    sampler.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sampler.shaderFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    sampler.bufferRange = 0;
    sampler.bufferOffset = 0;
    sampler.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    addUniformBufferBuildData(sampler);

    /*
    uboBuildData camera;
    camera.binding = 0;
    camera.descriptorCount = 1;
    camera.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    camera.shaderFlags = VK_SHADER_STAGE_VERTEX_BIT;
    camera.bufferOffset = 0;
    camera.bufferRange = sizeof(CameraUBO);
    camera.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    addUniformBufferBuildData(camera);

    uboBuildData lights;
    lights.binding = 1;
    lights.descriptorCount = 1;
    lights.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    lights.shaderFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    lights.bufferOffset = m_renderer->uboOffsetPadding(sizeof(CameraUBO));
    lights.bufferRange = sizeof(GlobalLightingUBO);
    lights.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    addUniformBufferBuildData(lights);

    uboBuildData sampler;
    sampler.binding = 2;
    sampler.descriptorCount = 1;
    sampler.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sampler.shaderFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    sampler.bufferRange = 0;
    sampler.bufferOffset = 0;
    sampler.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    addUniformBufferBuildData(sampler);

    uboBuildData model;
    model.binding = 3;
    model.descriptorCount = 1;
    model.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    model.shaderFlags = VK_SHADER_STAGE_VERTEX_BIT;
    model.bufferOffset = m_renderer->uboOffsetPadding(sizeof(CameraUBO) + sizeof(GlobalLightingUBO));
    model.bufferRange = sizeof(ModelUBO);
    model.poolDescriptorCount = getRenderer()->getSwapChainimagesSize();
    addUniformBufferBuildData(model);
    */

    printf("model oncreate1!\n");
    LoadModelIndexed();
    printf("model oncreate2!\n");
    m_renderer->createBufferForModel<Vertex>(m_vertices, m_vertexBuffer, m_vertexBufferMemory, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    printf("model oncreate3!\n");
    m_renderer->createBufferForModel<uint32_t>(m_indices, m_indexBuffer, m_indexBufferMemory, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    printf("model oncreate4!\n");
    m_renderer->CreateTextureImageAndView(m_textureName, m_textureImage, m_textureImageMemory, m_textureView);

    printf("model oncreate5!\n");
    m_isVertexed = true;
    printf("model oncreate6!\n");
    VulkanRenderableObject::OnCreate();

}

void Model::updateCommandBuffer(VkCommandBuffer& buffer, int& idx)
{
    VkBuffer vertexBuffers[] = { m_vertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(buffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSets[idx], 0, nullptr);

    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

    vkCmdDrawIndexed(buffer, m_indices.size(), 1, 0, 0, 0);
}

void Model::Update(
    const Matrix4& model,
    const Matrix4& normal,
    const Matrix4& view,
    const Matrix4& projection,
    const Vec4 position[3],
    const Vec4 color[3]
    )
{
    modelUniform.modelMatrix = model;
    modelUniform.modelNormalMatrix = normal;
    modelUniform.projectionMatrix = projection;
    modelUniform.viewMatrix = view;
    modelUniform.lightPos[0] = position[0];
    modelUniform.lightColor[0] = color[0];
    modelUniform.lightPos[1] = position[1];
    modelUniform.lightColor[1] = color[1];
    modelUniform.lightPos[2] = position[2];
    modelUniform.lightColor[2] = color[2];
    /*
    std::cout << "viewbol posX " << view[0] << std::endl;
    std::cout << "viewbol posY " << view[1] << std::endl;
    std::cout << "viewbol posZ " << view[2] << std::endl;
    std::cout << "viewbol colorX " << view[4] << std::endl;
    std::cout << "viewbol colorY " << view[5] << std::endl;
    std::cout << "viewbol colorZ " << view[6] << std::endl;
    */

    m_renderer->updateUniformBuffer(
        &modelUniform,
        m_uniformBufferMemoryMap,
        m_uboBuildData[0].bufferOffset,
        m_uboBuildData[0].bufferRange
    );

   /*
    m_renderer->updateUniformBuffer(
        &cameraUbo,
        m_uniformBufferMemoryMap,
        m_uboBuildData[0].bufferOffset,
        m_uboBuildData[0].bufferRange
    );
    m_renderer->updateUniformBuffer(
        &modelUbo,
        m_uniformBufferMemoryMap,
        m_uboBuildData[3].bufferOffset,
        m_uboBuildData[3].bufferRange
    );
    */
}
