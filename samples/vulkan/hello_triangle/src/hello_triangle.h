#pragma once

#include <vulkan/vulkan.h>
#include <shaderc/shaderc.hpp>

#define GLFW_INCLUE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class HelloTriangleApplication 
{
public:
    void run(uint32_t init_width, uint32_t init_height);

private:
    void initWindow(uint32_t width, uint32_t height);
    void initVulkan();
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createSwapChain();
    void createGraphicsPipeline();

    bool checkValidationLayerSupport() const;
    bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;
    std::vector<const char*> getRequiredExtensions() const;

    void compileShader(const char* sourceText, shaderc_shader_kind kind, std::vector<uint32_t>& outSpirv);
    VkShaderModule createShaderModule(const std::vector<uint32_t>& code);

    void mainLoop();
    void cleanup();

    VkInstance instance_ {VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT debugMessenger_ {VK_NULL_HANDLE};
    VkSurfaceKHR surface_ {VK_NULL_HANDLE};
    VkPhysicalDevice physicalDevice_ {VK_NULL_HANDLE};
    VkDevice device_ {VK_NULL_HANDLE};
    VkQueue graphicsQueue_ {VK_NULL_HANDLE};
    VkQueue presentQueue_ {VK_NULL_HANDLE};

    VkSwapchainKHR swapChain_ {VK_NULL_HANDLE};
    std::vector<VkImage> swapChainImages_;
    std::vector<VkImageView> swapChainImageViews_;
    VkFormat swapChainImageFormat_;
    VkExtent2D swapChainExtent_;

    VkPipelineLayout pipelineLayout_ {VK_NULL_HANDLE};

    GLFWwindow* window_ {nullptr};

    const std::vector<const char*> validationLayers_ = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions_ = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

#ifdef NDEBUG
    const bool enableValidationLayers_ = false;
#else
    const bool enableValidationLayers_ = true;
#endif
};