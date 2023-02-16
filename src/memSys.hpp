


#pragma once
#include "vk_mem_alloc.h"
#include <cstddef>
#include <immintrin.h>
#include <vulkan/vulkan_core.h>

struct [[clang::vecreturn, clang::trivial_abi]] memSys
{
    static inline void* data; //Moved to struct instead of inited in function due to teh abiloty to utilsie TaiLCalls when 
    static inline VmaAllocator vmaAllocator;
    memSys(VkPhysicalDevice physdevice, VkDevice device , VkInstance instance)
    {
        vmaAllocator=(doAlloc(physdevice, device , instance)) ;
    };
    static inline VmaAllocation reemptMappedSection;
    static auto doAlloc(VkPhysicalDevice, VkDevice, VkInstance) -> VmaAllocator;
    static auto aAlloc(VkPhysicalDevice, VkDevice, VkInstance) -> VmaAllocator;
    static auto allocBuf(size_t, VmaAllocation&) -> VkBuffer;
    static auto allocImg(VkExtent3D, size_t, VmaAllocation&) -> VkImage;
    [[gnu::pure, gnu::no_split_stack, clang::no_stack_protector, gnu::no_address_safety_analysis, gnu::flatten/* , gnu::force_align_arg_pointer] */]] static void mapMem(uint8_t*, VkDeviceSize);
    [[gnu::always_inline]] static void addMappedSection(VmaAllocation&, size_t);
};