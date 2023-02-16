

#include <cstdint>
#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_USE_64_BIT_PTR_DEFINES 1
#define VULKAN_HPP_SUPPORT_SPAN
#define WIN32_LEAN_AND_MEAN
#pragma once

using DWORD = unsigned long;
		using LPCWSTR = const wchar_t *;
		using HANDLE = void *;
		using HINSTANCE = struct HINSTANCE__*; //Make sure the Using.Tyodefs are defined/Aliased as a Pointer and not as an absolute handle/Reference.......[Confirm]
		using HWND = struct HWND__*;
		using HMONITOR = struct HMONITOR__*;
		using SECURITY_ATTRIBUTES = struct _SECURITY_ATTRIBUTES;
    
    
#include <vk_mem_alloc.h>
#include <vector>



// constexpr uint16_t width = 854;
// constexpr uint16_t height = 480;
// constexpr uint8_t Frames = 8;

constexpr bool ENABLE_VALIDATION_LAYERS = false;


constexpr auto *validationLayers = "VK_LAYER_KHRONOS_validation";

// TODO(CB): make this a Static/None Movable/Non-Copyable Instance/ (Ala Singleton Style Struct)
struct /* [[clang::trivial_abi]] */ VkInit
{
    // HINSTANCE inst;
    
    
    VkInstance instance=createInstance();
    VkSurfaceKHR surface;
    VkPhysicalDevice physdevice = doPhysicalDevice();
    VkDevice device = doDevice();
    // uint32_t graphicsFamily;
    // uint32_t transferFamily;
    VkQueue GraphicsQueue;
    // VkQueue PresentQueue;
    VkQueue TransferQueue;
    // SwapChain SW;
   explicit VkInit(HINSTANCE instance, HWND window): surface(createSurface(instance, window)){};
    
    VkInit(VkInit const &) = delete;
  auto operator=(const VkInit&) -> VkInit& = delete; 
     
  [[nodiscard]] auto  createInstance() const -> VkInstance;
     [[nodiscard]] auto createSurface(HINSTANCE, HWND) const -> VkSurfaceKHR;
     [[nodiscard]] auto  doPhysicalDevice() const -> VkPhysicalDevice;
     
     auto doDevice() -> VkDevice;
     template <typename name> auto vkEnumSet(auto a) const;
     template <typename name>  auto  vkEnumSet2(auto a) const -> std::vector<name>;
     [[nodiscard, gnu::pure, gnu::const, gnu::regcall]] constexpr auto tst() const noexcept -> VkDevice{ return this->device; };

} __attribute__((aligned(64)));
// VkInit VkInit::VKI;