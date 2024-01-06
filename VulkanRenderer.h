#ifndef VULKANRENDERER_H 
#define VULKANRENDERER_H
#define VK_ENABLE_BETA_EXTENSIONS

#include <SDL.h>
#include <SDL_vulkan.h>
#include <SDL_image.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <optional>
#include <set>
#include <unordered_map>
#include <array>
#include <chrono>
#include <memory>

#include "Structs.h"
#include "Model.h"
#include "DeviceHandler.h"
#include "PointLight.h"

using namespace MATH;

#include "Renderer.h"

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG /// only use validation layers if in debug mode
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class VulkanRenderer : public Renderer {
public:
    /// C11 precautions 
    VulkanRenderer(const VulkanRenderer&) = delete;  /// Copy constructor
    VulkanRenderer(VulkanRenderer&&) = delete;       /// Move constructor
    VulkanRenderer& operator=(const VulkanRenderer&) = delete; /// Copy operator
    VulkanRenderer& operator=(VulkanRenderer&&) = delete;      /// Move operator

    VulkanRenderer();
    ~VulkanRenderer();
    bool OnCreate();
    SDL_Window* CreateWindow(std::string name_, int width_, int height_) { return nullptr; };
    void OnDestroy();
    void Render();
    void SetCameraUBO(const Matrix4& view, const Matrix4& projection);
    void SetGlobalLightingUBO(const Vec4& position, const Vec4& diffuse);
    void SetModelUBO(const Matrix4& model, const Matrix4& normal);
    SDL_Window* GetWindow() { return window; }
    void CreateTextureImage();
    void CreateGraphicsPipeline();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDeviceSize& offset);

    std::unique_ptr<PointLight> light1{ nullptr };

    // we need these functions to be public for the "actor" classes
    inline uint32_t getSwapChainimagesSize() { return static_cast<uint32_t>(swapChainImages.size()); };
    inline VkSampler getImageSampler() { return textureSampler; };
    VkDescriptorImageInfo getImageInfoDescriptor();

    size_t uboOffsetPadding(size_t offset);
    size_t vertexStridePadding(size_t offset);
    uint32_t m_imageIndex{0};

    void CreateGraphicsPipeline(
        const std::string& vertFilePath,
        const std::string& fragFilePath,
        VkDescriptorSetLayout& descriptorSetLayout,
        VkPipelineLayout& pipelineLayout,
        VkPipeline& graphicsPipeline,
        bool isVertexed,
        bool isPushConstantUsed,
        bool withGeometryShader,
        const std::string& geomtFilePath
    );
    void createDescriptorSetLayout(
        VkDescriptorSetLayout& descriptorSetLayout,
        std::vector<VkDescriptorSetLayoutBinding>& bindings
    );
    void createDescriptorPool(
        VkDescriptorPool& descriptorPool,
        std::vector<VkDescriptorPoolSize>& poolSizes
    );
    void createUniformBuffers(
        VkDeviceSize& sizeOfBuffer,
        std::vector<VkBuffer>& uniformBuffers,
        std::vector<VkDeviceMemory>& uniformBuffersMemory,
        std::vector<void*>& uniformBufferMemoryMap
    );
    void createDescriptorSets(
        VkDescriptorSetLayout& descriptorSetLayout,
        VkDescriptorPool& descriptorPool,
        std::vector<VkDescriptorSet>& descriptorSets
    );
    void updateDescriptorSets(
        std::vector<VkWriteDescriptorSet>& descriptorWrites
    );
    void cleanupDescriptorSetLayout(VkDescriptorSetLayout& layout);
    void cleanupPipeline(VkPipeline& pipeline);
    void cleanupDescriptorPool(VkDescriptorPool& pool);
    void cleanupBuffer(VkBuffer& buffer);
    void cleanupMemory(VkDeviceMemory& memory);
    void cleanupCommandBuffer(std::vector<VkCommandBuffer>& cmdBuffer);

    void mapMemory(
        std::vector<void*>& memoryMap,
        std::vector<VkDeviceMemory>& memory
    )
    {
        for (int i = 0; i < memory.size(); i++)
        {
            void* data;
            vkMapMemory(device, memory[i], 0, VK_WHOLE_SIZE, 0, &data);
            memoryMap.push_back(data);
        }
    }

    void updateUniformBuffer(
        void* ubos,
        std::vector<void*>& memoryMap,
        VkDeviceSize offset,
        VkDeviceSize size
    )
    {
        char *pos = (char*) memoryMap[m_imageIndex];
        pos += (size_t)offset;
        memcpy(pos, ubos, (size_t)size);
    };

    void CreateTextureImageAndView(
    const char* textureName,
    VkImage& textureImage,
    VkDeviceMemory& textureBufferMemory,
    VkImageView& textureView
    );

template <typename T>
void createBufferForModel(
    const std::vector<T>& modelData,
    VkBuffer& modelBuffer,
    VkDeviceMemory& modelBufferMemory,
    VkBufferUsageFlags flags
    ) {
    VkDeviceSize bufferSize = sizeof(modelData[0]) * modelData.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
                                                                                                                       // these are being pulled in by reference, be constatant
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, modelData.data(), (size_t)bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize, flags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, modelBuffer, modelBufferMemory);

    copyBuffer(stagingBuffer, modelBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

    void createCommandBuffers();
    void createSecondaryCommandBuffersStart(std::vector<VkCommandBuffer>& cmdBuffer);
    void createSecondaryCommandBuffersFinish(std::vector<VkCommandBuffer>& cmdBuffer);
    void addCommandToCommandBuffer(std::vector<VkCommandBuffer>& newBuffer);

private:
    const size_t MAX_FRAMES_IN_FLIGHT = 2;

    // model data storing
    std::unique_ptr<Model> mario{ nullptr };
    std::unique_ptr<DeviceHandler> DH{ nullptr };

    SDL_Event sdlEvent;
    uint32_t windowWidth;
    uint32_t windowHeight;
    SDL_Window* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VkPhysicalDeviceProperties m_physicalDeviceProperties{};

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkCommandPool commandPool;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<std::vector<VkCommandBuffer>> m_commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    bool framebufferResized = false;

    
    

    bool hasStencilComponent(VkFormat format);

    void initVulkan();
    void createSwapChain();
    void createImageViews();
    void recreateSwapChain();
    void updateUniformBuffer(uint32_t currentImage);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
    void createRenderPass();
    void createDescriptorSetLayout();
   
    void createFramebuffers();
    void createCommandPool();
    void createDepthResources();
   
    void createTextureImageView();
    void createTextureSampler();
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
   
    void createVertexBuffer(const std::vector<Vertex>& modelVertices);
        /// A helper function for createVertexBuffer()
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createIndexBuffer(const std::vector<uint32_t>& modelIndices);
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void createSyncObjects();
    void cleanup();
    void cleanupSwapChain();
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    
    CameraUBO cameraUbo;
    GlobalLightingUBO globalLightingUbo;
    ModelUBO marioModelUbo;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    static std::vector<char> readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

};
#endif 

