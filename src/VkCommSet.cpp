#include "VkCommSet.hpp"
#include <cassert>
#include <cstdint>
#include <vulkan/vulkan_core.h>
 
void VkCommSet::beginSingleTimeCommands()
{
     VkCommandBufferBeginInfo beginInfo1 = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                                                    .flags = ( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };

                                                
  chkTst(vkBeginCommandBuffer(this->commandBuffer, &beginInfo1));
}
VkCommandBuffer VkCommSet::doGenCommnd(VkCommandPool commandPool)
{
  VkCommandBuffer PreTestBuffer;
  assert(commandPool);
   const VkCommandBufferAllocateInfo allocateInfo{ .sType              =  VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO ,
                                                  .commandPool        =  commandPool ,
                                                  .level              =  VK_COMMAND_BUFFER_LEVEL_PRIMARY ,
                                                  .commandBufferCount = 1};

  chkTst(vkAllocateCommandBuffers(Vks::Device, &allocateInfo, &PreTestBuffer ));
  assert(PreTestBuffer);
  return PreTestBuffer;
}
VkCommandPool VkCommSet::genCommPool(uint32_t QueueFamilyIndex)
{
  VkCommandPoolCreateInfo  poolInfo = {
    .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext            = nullptr,
    .flags=VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = QueueFamilyIndex,
  };
  return Vks::doPointerAlloc5<VkCommandPool>( &poolInfo, vkCreateCommandPool );
 
}

void VkCommSet::endSingleTimeCommands(VkQueue queue)
{
  chkTst(vkEndCommandBuffer( commandBuffer ));
  const VkSubmitInfo       submitInfo1 = {
          .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, .pNext = VK_NULL_HANDLE, .commandBufferCount = ( 1 ), .pCommandBuffers = &commandBuffer
  };
//   a = ( a ^ 1 );
  chkTst(vkQueueSubmit( queue, 1, &submitInfo1, VK_NULL_HANDLE ));
  // chkTst(vkQueueWaitIdle( queue ));
//   vkResetCommandPool( Vks::Device, ( commandPool2 ), VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT );
}