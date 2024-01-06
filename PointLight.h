#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "VulkanRenderableObject.h"

class PointLight : public VulkanRenderableObject
{
private:
    pointLightUboData lightUbo;
    lightPushC lightConstant;

public:
    PointLight(
        VulkanRenderer* renderer
    ) :
    VulkanRenderableObject(renderer, "shaders/lightPoint_vert.spv", "shaders/lightPoint_frag.spv") {};
    void OnCreate();
    void Update(const MATH::Vec4& newPos, const MATH::Matrix4& view, const MATH::Matrix4& projection);
    void updateCommandBuffer(VkCommandBuffer& buffer, int& idx) override;

    inline void setColor(const Vec4& newcolor) { lightConstant.color = newcolor; };

};

#endif // !POINT_LIGHT_H
