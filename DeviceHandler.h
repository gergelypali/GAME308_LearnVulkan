#ifndef DEVICE_HANDLER_H
#define DEVICE_HANDLER_H

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <vector>
#include <SDL.h>
#include <iostream>
#include "Structs.h"

class DeviceHandler
{
private:
	bool m_enableValidationLayers{ true };
	VkDebugUtilsMessengerEXT m_debugMessenger;

	SDL_Window* m_window;
	VkSurfaceKHR m_surface;
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_logicalDevice;
	VkInstance m_instance;
	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	const std::vector<const char*> m_validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> m_deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
	};

	bool checkValidationLayerSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();

public:
	void CreateWindow(std::string name_, int width_, int height_);
	void createSurface();
	void createInstance();
	void setupDebugMessenger();
	void pickPhysicalDevice();
	void createLogicalDevice();

	// helper functions
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	// Getters
	inline VkPhysicalDevice GetPhysicalDevice() { return m_physicalDevice; };
	inline VkDebugUtilsMessengerEXT GetDebugMessenger() { return m_debugMessenger; };
	inline VkDevice GetLogicalDevice() { return m_logicalDevice; };
	inline VkInstance GetInstance() { return m_instance; };
	inline VkQueue GetGraphicsQueue() { return m_graphicsQueue; };
	inline VkQueue GetPresentQueue() { return m_presentQueue; };
	inline SDL_Window* GetWindow() { return m_window; };
	inline VkSurfaceKHR GetSurface() { return m_surface; };
};

#endif // !DEVICE_HANDLER_H
