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
    static void glfw_error_callback(int error, const char* description)
    {
        spdlog::error(ERROR_MSG_GLFW_ERROR, description);
    }

    void initCore()
    {
#ifdef KIFF_DEBUG
        spdlog::info(DEBUG_MSG_APP_START);
#endif

        if (glfwInit() != GLFW_TRUE) 
        {
            spdlog::error(ERROR_MSG_GLFW_INIT_FAILED);
            std::exit(EXIT_FAILURE);
        }

#ifdef KIFF_DEBUG
        spdlog::info(DEBUG_GLFW_INIT);
#endif

        glfwSetErrorCallback(glfw_error_callback);

        auto instanceInfo = AppContext::InstanceInfo
        {
            .app_name = "Test App",
            .app_version = VK_MAKE_API_VERSION(0,1,0,0),
            .engine_name = "Test Engine",
            .engine_version = VK_MAKE_API_VERSION(0,1,0,0)
        };

        auto create_instance_result = AppContext::createInstance(instanceInfo);

        if(create_instance_result.getCode() != KIFF_SUCCESS)
        {
            spdlog::error(ERROR_MSG_CREATE_INSTANCE_FAILED, create_instance_result.getResultCodeValue());
            std::exit(EXIT_FAILURE);
        }

        auto app_ctx = AppContext::Context{};
        app_ctx.instance = create_instance_result.getValue();

#ifdef KIFF_DEBUG
        spdlog::info(DEBUG_MSG_INSTANCE_CREATED);

        auto createDebugMessengerResult = createDebugMessenger(app_ctx.instance);
        if (createDebugMessengerResult.getCode() == KIFF_SUCCESS)
        {
            app_ctx.debug_messenger = createDebugMessengerResult.getValue();
        }

        spdlog::info(DEBUG_MSG_MESSENGER_CREATED);
#endif

        auto render_ctx = RenderContext{};

        auto select_physical_device_result = selectPhysicalDevice(app_ctx.instance);
        if (select_physical_device_result.getCode() == KIFF_SUCCESS)
        {
            render_ctx.physical_device = select_physical_device_result.getValue();
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(640, 480, "Test App", NULL, NULL);

        VkSurfaceKHR surface;
        VkResult err = glfwCreateWindowSurface(app_ctx.instance, window, NULL, &surface);
        if (err)
        {
            spdlog::error(ERROR_MSG_CREATE_SURFACE_FAILED, string_VkResult(err));
        }

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }

        vkDestroySurfaceKHR(app_ctx.instance, surface, nullptr);
        
        glfwDestroyWindow(window);
        glfwTerminate();

#ifdef KIFF_DEBUG
         spdlog::info(DEBUG_MSG_APP_EXIT);
#endif
    }
}