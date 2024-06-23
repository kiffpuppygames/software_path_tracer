#pragma once

#include <string>
#include <optional>
#include <tuple>

#include "vulkan/vulkan_core.h"
#include "GLFW/glfw3.h"
  
#include "globals.h"
#include "result.h"

struct AppContext
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT vkDebugMessenger;
};

struct InstanceInfo
{
    std::string applicationName;
    uint32_t applicationVersion;
    std::string engineName;
    uint32_t engineVersion;
};

Result<VkInstance> createInstance(InstanceInfo instanceInfo)
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = instanceInfo.applicationName.c_str();
    appInfo.applicationVersion = instanceInfo.applicationVersion;
    appInfo.pEngineName = instanceInfo.applicationName.c_str();
    appInfo.engineVersion = instanceInfo.engineVersion;
    appInfo.apiVersion = VULKAN_API_VERSION_PINNED;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;    
    createInfo.enabledLayerCount = 0;

#ifndef NDEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    bool isValidationSupported = true;
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : VALIDATION_LAYERS) 
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) 
        {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) 
        {
            return Result<VkInstance>(KIFF_VALIDATION_LAYER_NOT_PRESENT);
        }
    }

    spdlog::info(DEBUG_MSG_EXTENSIONS_REQ, glfwExtensionCount);

    for (uint32_t i = 0; i < glfwExtensionCount; ++i) 
    {
        spdlog::info(DEBUG_INDENT_1, glfwExtensions[i]);
    }
    createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#endif

    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    switch (result)
    {
        case VK_SUCCESS:
            return Result<VkInstance>(KIFF_SUCCESS, std::make_unique<VkInstance>(instance));
            break;
        default:
            return Result<VkInstance>(KIFF_UNKNOWN);
            break;
    }
};