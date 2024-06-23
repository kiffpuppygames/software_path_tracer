#include "app_context.h"

#include "debug.h"

namespace AppContext
{
    Result<VkInstance> createInstance(InstanceInfo instanceInfo)
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = instanceInfo.app_name.c_str();
        appInfo.applicationVersion = instanceInfo.app_version;
        appInfo.pEngineName = instanceInfo.engine_name.c_str();
        appInfo.engineVersion = instanceInfo.engine_version;
        appInfo.apiVersion = VULKAN_API_VERSION_PINNED;

        uint32_t glfw_extension_count = 0;
        const char** glfw_extensions;
        glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

        VkInstanceCreateInfo instance_create_info{};
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pApplicationInfo = &appInfo;
        instance_create_info.enabledLayerCount = 0;

#ifdef KIFF_DEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        bool isValidationSupported = true;
        std::vector<VkLayerProperties> availableLayers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, availableLayers.data());

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

        spdlog::info(DEBUG_MSG_EXTENSIONS_REQ, glfw_extension_count);

        for (uint32_t i = 0; i < glfw_extension_count; ++i)
        {
            spdlog::info(DEBUG_INDENT_1, glfw_extensions[i]);
        }
        instance_create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        instance_create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#endif

        instance_create_info.enabledExtensionCount = (uint32_t)extensions.size();
        instance_create_info.ppEnabledExtensionNames = extensions.data();

        VkInstance instance;
        VkResult result = vkCreateInstance(&instance_create_info, nullptr, &instance);

        switch (result)
        {
        case VK_SUCCESS:
            return Result<VkInstance>(KIFF_SUCCESS, instance);
            break;
        default:
            return Result<VkInstance>(KIFF_UNKNOWN);
            break;
        }
    };

    void Context::cleanup_context() const
    {
#ifdef KIFF_DEBUG
        destroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
#endif
        vkDestroyInstance(instance, nullptr);
    }
}
