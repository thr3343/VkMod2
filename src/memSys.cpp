#include "memSys.hpp"
#include "vk_mem_alloc.h"
#include "Vks.tpp"
#include <cstddef>
#include <cstdint>
#include <immintrin.h>
#include <vulkan/vulkan_core.h>

constexpr auto getMaxBARSize(auto &mheaps) -> VkDeviceSize
{
    auto xl=0;
    
    for(const auto memTypes : mheaps.memoryTypes)
   {
    
    printf("Type %u, \n", memTypes.heapIndex);
    printf("Size %u, \n", memTypes.propertyFlags);
    //Windows for some reason does not allocate/provide the full 256MB allowed by the default BAR if Resizeable BAR is not enabled/Supported
    xl=(memTypes.propertyFlags==(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) ? memTypes.heapIndex : xl;
    
   }
   return mheaps.memoryHeaps[xl].size;
}

auto memSys::doAlloc(VkPhysicalDevice physdevice, VkDevice device , VkInstance instance) -> VmaAllocator
{

    VkPhysicalDeviceMemoryProperties  vkPhysicalDeviceProperties;

    vkGetPhysicalDeviceMemoryProperties(physdevice, &vkPhysicalDeviceProperties);

   constexpr VkMemoryHeapFlags x={VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
   const auto aa= reinterpret_cast<VkDeviceSize*>(&vkPhysicalDeviceProperties.memoryHeapCount); 
   VkDeviceSize MAX_BAR=getMaxBARSize(vkPhysicalDeviceProperties);
    printf("memHeap %llu, \n", MAX_BAR);
    printf("sizeLimit %llu, \n", *aa);
auto xx= 0xFFFFFFFF;
   VmaVulkanFunctions vmaVulkanFunctions;
  VmaAllocatorCreateInfo VmaAllocationCreateInfo
  {
    .physicalDevice=physdevice,
    .device=device,
    .preferredLargeHeapBlockSize=MAX_BAR,
    .pHeapSizeLimit=aa,
    //.pVulkanFunctions=&vmaVulkanFunctions,
    .instance=instance,
    .vulkanApiVersion=VK_API_VERSION_1_3,
  };


  vmaCreateAllocator(&VmaAllocationCreateInfo, &vmaAllocator);
  return vmaAllocator;
}
/* auto aAlloc(VkPhysicalDevice, VkDevice, VkInstance) -> VmaAllocator
{
  VmaAllocationCreateInfo VmaAllocationCreateInfo
  {
    .usage=VMA_MEMORY_USAGE_AUTO,
    .memoryTypeBits=VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    
  };
} */

VkBuffer memSys::allocBuf(/* VkBuffer buffer,  */size_t size, VmaAllocation &vmaAllocation)
{
  constexpr uint32_t TransferQueCount=0U;
  std::cout << "AllocateSize: " << size << "\n";
  VkBufferCreateInfo VkBufferCreateInfo
  {
    .sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    .size=size,
    .usage=VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    .sharingMode=VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount=1,
    .pQueueFamilyIndices=&TransferQueCount,
  };
  // VmaAllocation vmaAllocation;
  VmaAllocationCreateInfo VmaAllocationCreateInfo
  {
    .flags=VMA_ALLOCATION_CREATE_MAPPED_BIT|VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
    .usage=VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
    .memoryTypeBits=0,
    .pool=nullptr
  };
  // vmaCreatePool(0, 0, 0)
  VkBuffer buffer;
  chkTst(vmaCreateBuffer(vmaAllocator, &VkBufferCreateInfo, &VmaAllocationCreateInfo, &buffer, &vmaAllocation, nullptr));
  //vmaBindImageMemory(vmaAllocator)
  return buffer;
}
VkImage memSys::allocImg(VkExtent3D extent, size_t size, VmaAllocation &vmaAllocation)
{
  constexpr uint32_t TransferQueCount=0U;
  std::cout << "AllocateSize: " << size << "\n";
  VkImageCreateInfo VkImageCreateInfo
  {
    .sType=VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .imageType=VK_IMAGE_TYPE_2D,
    .format=VK_FORMAT_R8G8B8A8_UNORM,
    .extent=extent,
    .mipLevels=1,
    .arrayLayers=1,
    .samples=VK_SAMPLE_COUNT_1_BIT,
    .tiling=VK_IMAGE_TILING_LINEAR,
    .usage=VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    .sharingMode=VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount=1,
    .pQueueFamilyIndices=&TransferQueCount,
    .initialLayout=VK_IMAGE_LAYOUT_UNDEFINED,
  };
  // VmaAllocation vmaAllocation;
  VmaAllocationCreateInfo VmaAllocationCreateInfo
  {
    .flags=VMA_ALLOCATION_CREATE_MAPPED_BIT|VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
    .usage=VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
    .memoryTypeBits=0,
    .pool=nullptr
  };
  VkImage image;
  chkTst(vmaCreateImage(vmaAllocator, &VkImageCreateInfo, &VmaAllocationCreateInfo, &image, &vmaAllocation, nullptr));
  //vmaBindImageMemory(vmaAllocator)
  return image;
}

void memSys::mapMem(uint8_t *limg, VkDeviceSize imageSize)
{
	
    {

	    memcpy(data, limg, imageSize);

    }
    // vmaUnmapMemory(vmaAllocator, VmaAllocation);
}

// void memSys::addMappedSection(VmaAllocation& VmaAllocation, size_t)
// {
//   // reemptMappedSection=&VmaAllocation;
//   chkTst(vmaMapMemory(vmaAllocator, *reemptMappedSection, (&data)));
// }