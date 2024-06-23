#pragma once

#include <string>
#include <optional>
#include <tuple>

#include "vulkan/vulkan_core.h"
#include "GLFW/glfw3.h"

#include "result.h"

namespace AppContext
{
    struct InstanceInfo
    {
        std::string app_name;
        uint32_t app_version;
        std::string engine_name;
        uint32_t engine_version;
    };

    struct Context
    {
        VkInstance instance;
        VkDebugUtilsMessengerEXT debug_messenger;

        void cleanup_context() const;

        ~Context()
        {
            cleanup_context();
        };
    };

    Result<VkInstance> createInstance(InstanceInfo instanceInfo);
}