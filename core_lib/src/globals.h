// @file

#pragma once

#include <string>
#include <vector>

#include "vulkan/vulkan_core.h"

const uint32_t VULKAN_API_VERSION_PINNED = VK_API_VERSION_1_3;

const std::vector<const char*> VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
    constexpr const char* VK_VALIDATION_LAYER = "VK VALIDATION LAYER: {}";
    constexpr const char* DEBUG_GLFW_INIT = "GLFW Initilized.";
    constexpr const char* DEBUG_INDENT_1 = "\t{}";
    constexpr const char* DEBUG_MSG_EXTENSIONS_REQ = "{} extensions required by GLFW";
    constexpr const char* DEBUG_MSG_APP_START = "App Started";
    constexpr const char* DEBUG_MSG_APP_EXIT = "App Exited.";
    constexpr const char* DEBUG_MSG_MESSENGER_CREATED = "Debug Messenger Created.";
    constexpr const char* DEBUG_MSG_INSTANCE_CREATED = "Instance Created.";
    constexpr const char* DEBUG_MSG_PHYSICAL_DEVICE_PRESENT = "Present Physical Devices:";
    constexpr const char* DEBUG_MSG_SELECTED_DEVICE = "Selected Physical Device: {}";
#endif

constexpr const char* ERROR_MSG_GLFW_ERROR = "GLFW Error: %s\n";
constexpr const char* ERROR_MSG_GLFW_INIT_FAILED = "GLFW Failed to initilize.";
constexpr const char* ERROR_MSG_VALIDATION_LAYER = "VK VALIDATION LAYER: {}";
constexpr const char* ERROR_MSG_CREATE_INSTANCE_FAILED = "Failed top create Vulkan Instance: {}";
constexpr const char* ERROR_MSG_CREATE_SURFACE_FAILED = "Failed top create window surface. {}";
constexpr const char* ERROR_RESULT_NO_CODE_FOUND = "No code found";
constexpr const char* ERROR_RESULT_NO_VALUE_FOUND = "No value found";
constexpr const char* ERROR_CREATE_LOGICAL_DEVICE = "Failed to create logical device";