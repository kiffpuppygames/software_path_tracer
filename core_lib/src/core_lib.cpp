#include <iostream>

#include "vulkan/vulkan_core.h"
#include "vulkan/vk_enum_string_helper.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "globals.h"
#include "result.h"
#include "debug.h"
#include "app_context.h"
#include "render_context.h"

#include "core_lib.h"

namespace Core 
{
    void glfw_error_callback(int error, const char* description)
    {
        spdlog::error(ERROR_MSG_GLFW_ERROR, description);
    }

    int main()
    {
#ifndef NDEBUG
        spdlog::info(DEBUG_MSG_APP_START);
#endif

        if (glfwInit() != GLFW_TRUE) 
        {
            spdlog::error(ERROR_MSG_GLFW_INIT_FAILED);
            std::exit(EXIT_FAILURE);
        }

#ifndef NDEBUG
        spdlog::info(DEBUG_GLFW_INIT);
#endif

        glfwSetErrorCallback(glfw_error_callback);

        auto instanceInfo = InstanceInfo
        {
            .applicationName = "Test App",
            .applicationVersion = VK_MAKE_API_VERSION(0,1,0,0),
            .engineName = "Test Engine",
            .engineVersion = VK_MAKE_API_VERSION(0,1,0,0)
        };

        auto create_instance_result = createInstance(instanceInfo);

        if(create_instance_result.getCode() != KIFF_SUCCESS)
        {
            spdlog::error(ERROR_MSG_CREATE_INSTANCE_FAILED, create_instance_result.getResultCodeValue());
            std::exit(EXIT_FAILURE);
        }

        auto appCtx = AppContext{};
        appCtx.instance = create_instance_result.getValue();

#ifndef NDEBUG
        spdlog::info(DEBUG_MSG_INSTANCE_CREATED);

        auto createDebugMessengerResult = createDebugMessenger(appCtx.instance);
        if (createDebugMessengerResult.getCode() == KIFF_SUCCESS)
        {
            appCtx.vkDebugMessenger = createDebugMessengerResult.getValue();
        }

        spdlog::info(DEBUG_MSG_MESSENGER_CREATED);
#endif

        auto renderCtx = RenderContext{};

        auto select_physical_device_result = selectPhysicalDevice(appCtx.instance);
        if (select_physical_device_result.getCode() == KIFF_SUCCESS)
        {
            renderCtx.physicalDevice = select_physical_device_result.getValue();
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(640, 480, "Test App", NULL, NULL);

        VkSurfaceKHR surface;
        VkResult err = glfwCreateWindowSurface(appCtx.instance, window, NULL, &surface);
        if (err)
        {
            spdlog::error(ERROR_MSG_CREATE_SURFACE_FAILED, string_VkResult(err));
        }

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }

#ifndef NDEBUG
        destroyDebugUtilsMessengerEXT(appCtx.instance, appCtx.vkDebugMessenger, nullptr);
#endif

        vkDestroySurfaceKHR(appCtx.instance, surface, nullptr);
        vkDestroyInstance(appCtx.instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();

#ifndef NDEBUG
         spdlog::info(DEBUG_MSG_APP_EXIT);
#endif
    }

    void PrintHelloWorld()
    {
        spdlog::info("Hello World");
    };
}