#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "VulkanRenderableObject.h"

#include "Structs.h"

class Model : public VulkanRenderableObject
{
private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    const char* m_fileName{};
    const char* m_textureName{};

    modelUboData modelUniform;

    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
    VkBuffer m_indexBuffer;
    VkDeviceMemory m_indexBufferMemory;
	VkPipeline m_graphicsPipelineForNormals;

public:
    Model(
        VulkanRenderer* renderer,
        const char* fileName,
        const char* textureName
        ) :
    m_fileName(fileName),
    m_textureName(textureName),
    VulkanRenderableObject(
        renderer,
        "shaders/phong_vert.spv",
        "shaders/phong_frag.spv"
        ) {};

    void OnCreate();
    void updateCommandBuffer(VkCommandBuffer& buffer, int& idx) override;
    void Update(
        const Matrix4& model,
        const Matrix4& normal,
        const Matrix4& view,
        const Matrix4& projection,
        const Vec4 position[3],
        const Vec4 color[3]
    );

    void LoadModelIndexed();

    inline std::vector<Vertex> GetVertices() { return m_vertices; }
    inline std::vector<uint32_t> GetIndices() { return m_indices; }
};

#endif // !MODEL_H
