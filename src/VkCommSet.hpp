#pragma once
#include "Vks.tpp"
#include <cstdint>

struct VkCommSet
{
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkCommandPool genCommPool(uint32_t);
    VkCommandBuffer doGenCommnd(VkCommandPool);
    explicit VkCommSet(uint32_t i): commandPool(genCommPool(i)),commandBuffer(doGenCommnd(commandPool)){};
    void beginSingleTimeCommands();
    void endSingleTimeCommands(VkQueue queue);
} __attribute__((aligned(16)));