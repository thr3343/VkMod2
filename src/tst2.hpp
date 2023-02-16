#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>
#include "VkCommSet.hpp"
#include <array>
#include "vk_mem_alloc.h"
#include "Vks.tpp"

constexpr VkImageLayout dstLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
constexpr VkImageLayout srcLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
constexpr VkImageUsageFlags srcBit = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
constexpr VkImageUsageFlags dstBit = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

static constexpr VkImageSubresourceLayers subresource = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
	    .mipLevel = 0,
	    .baseArrayLayer = 0,
	    .layerCount = 1,
        };


struct tst2
{
    VkQueue TransferQueue2;
    VkImage image;
    // VkImage DstImg;
    // VkDeviceMemory memory;
        VmaAllocation VmaAllocation;
    VkOffset2D Offs;
    uint32_t xi, yi, cnls;
    VkBuffer stagingBuffer;
    // VkCommandBuffer transferCommBuffers;
    std::array<VkCommandBuffer, Frames> commandBuffers;
    explicit tst2(VkQueue queue, std::array<VkCommandBuffer, Frames> commandBuffers) 
    : 
        TransferQueue2(queue), 
        // transferCommBuffers(transferCommBuffers),
        commandBuffers(commandBuffers), 
        image(setupImagetest())
        // DstImg(createEmptImg())
        {};

    VkImage setupImagetest();
    void transitionImageLayout( VkCommandBuffer commandBuffer, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImage &image );
    auto copyBufToImg( VkBuffer buffer, VkImage image, VkExtent3D extent);
    void copyImgToBuf( VkImage& img );
    void vkRecImg(VkImage &img, int a);
    VkImage createEmptImg();


};