#include "SwapChain.hpp"
#include <vulkan/vulkan_core.h>


auto SwapChain::getCurrentSwapChainSurfaceCapabilities() const -> VkExtent2D
{
   VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physdevice, surface, &capabilities );
  std::cout <<"maxImageCount"<< capabilities.maxImageCount<<"\n";
  std::cout <<"supportedUsageFlags"<< capabilities.supportedUsageFlags<<"\n";
  printf("minExtent:Width, %i %i\n",capabilities.minImageExtent.width, capabilities.minImageExtent.height);
  printf("maxExtent:Width, %i %i\n",capabilities.maxImageExtent.width, capabilities.maxImageExtent.height);
  //std::cout <<"maxExtent"<< capabilities.maxImageExtent<<"\n";
  return capabilities.currentExtent;
}

auto SwapChain::setupImageFormats() -> VkSurfaceFormatKHR
{
    std::cout <<"SetupImageFormats"<<"\n";
    uint32_t count=0;
    

    vkGetPhysicalDeviceSurfaceFormatsKHR(physdevice, surface, &count, nullptr );
    std::vector<VkSurfaceFormatKHR> surfaceFormats(count);
    
    {
      vkGetPhysicalDeviceSurfaceFormatsKHR(physdevice, surface, &count, surfaceFormats.data() );
    }
    vkGetPhysicalDeviceSurfacePresentModesKHR(physdevice, surface, &count, nullptr );
    std::vector<VkPresentModeKHR> presentModes(count);

    {
      vkGetPhysicalDeviceSurfacePresentModesKHR(physdevice, surface, &count, presentModes.data() );
    }

    VkSurfaceFormatKHR         swapChainImageFormat;

    // VkSurfaceFormatKHR surfaceFormat;
    for ( const VkSurfaceFormatKHR & surfaceFormat1 : surfaceFormats )
    {
      swapChainImageFormat= ( surfaceFormat1.format == VK_FORMAT_B8G8R8A8_UNORM && surfaceFormat1.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)?surfaceFormat1:surfaceFormats[0];
    
    }

    for ( const VkPresentModeKHR & presentMode1 : presentModes )
    {
      
        presentMode = (presentMode1  == VK_PRESENT_MODE_IMMEDIATE_KHR)? VK_PRESENT_MODE_IMMEDIATE_KHR : VK_PRESENT_MODE_FIFO_KHR;
       
    }

    

    return swapChainImageFormat;
}


auto SwapChain::getSwapChainImages(uint32_t size) -> std::array<VkImage, Frames>
{
  std::array<VkImage, Frames> image;

  VkImageCreateInfo VkImageCreateInfo{
    .sType=VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .flags=VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT|VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT,
    .imageType=VK_IMAGE_TYPE_2D,
    .format=VK_FORMAT_BC1_RGB_UNORM_BLOCK,
    .extent{width,height,1},
    .mipLevels=1,
    .arrayLayers=1,
    .samples=VK_SAMPLE_COUNT_1_BIT,
    .tiling=VK_IMAGE_TILING_OPTIMAL,
    .usage=VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    .queueFamilyIndexCount=1,
    .pQueueFamilyIndices=0,
    .initialLayout=VK_IMAGE_LAYOUT_PREINITIALIZED
  };

  vkCreateImage(Vks::Device, &VkImageCreateInfo, nullptr, image.data());

  vkGetSwapchainImagesKHR( Vks::Device, swapChain, &size, image.data());
  return image;
}

auto SwapChain::createSwapChain(const VkSurfaceFormatKHR swapChainImageFormat) -> VkSwapchainKHR
  {
    std::cout << "ImageCount: " << Frames << "\n";
    

    const VkSwapchainCreateInfoKHR createInfo{

      .sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .pNext   = nullptr,
      .surface = surface,

      // Image settings
      .minImageCount    = Frames,
      .imageFormat      = swapChainImageFormat.format,
      .imageColorSpace  = swapChainImageFormat.colorSpace,
      .imageExtent      = swapChainExtent,
      .imageArrayLayers = 1,
      .imageUsage       = VK_IMAGE_USAGE_TRANSFER_DST_BIT,

      .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE, //Is concurrent even needed in many cases...
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices   = 0,
      

      .preTransform   = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .presentMode    = presentMode,
      .clipped        = true,

      .oldSwapchain = VK_NULL_HANDLE
    };
    std::cout << Vks::Device << "\n";
    VkSwapchainKHR swapChain;
    vkCreateSwapchainKHR(Vks::Device, &createInfo, nullptr, &swapChain );
    return swapChain;
    
  }

  


//Posibel BUg fix: Don't use self-reference to the parent Struct vkImage Array and instead just use a passed VkImage paramter so it isn't accidentally Overridden!
  auto SwapChain::createImageViews(std::array<VkImage, Frames> images) -> std::array<VkImageView, Frames>
  {
    std::cout << ( "Creating Image Views" ) << "\n";
    int i = 0;
    std::array<VkImageView, Frames> imageViews;
    for ( const VkImage & swapchainImage : images )
    {
      VkImageViewCreateInfo createInfo = {};
      createInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format   = swapChainImageFormat.format;

      //     createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      // createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      // createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      //     createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

      createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel   = 0;
      createInfo.subresourceRange.levelCount     = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount     = 1;

      createInfo.image = swapchainImage;

      imageViews[i]=Vks::doPointerAlloc5<VkImageView>(&createInfo, vkCreateImageView);
      i++;
    }
    return imageViews;
  }

  auto SwapChain::createFramebuffers(VkRenderPass renderpass) -> VkFramebuffer
  {
        std::cout << ( "Creating FrameBuffers" ) << "\n";

    // framebufferCreateInfo.attachmentCount = 1;
    
    VkFramebufferAttachmentImageInfo FramebufferAttachmentImage
    {
      .sType=VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO,
      .usage=VK_IMAGE_USAGE_TRANSFER_DST_BIT, //Nvidia Driver bug with Usages/Varients is now fixed in an eailer 473.** Driver Branch and does/no longer needs an offset tp be corrected manually
      .width= swapChainExtent.width,
      .height= swapChainExtent.height,
      .layerCount=1,
      .viewFormatCount=1,
      .pViewFormats=&swapChainImageFormat.format
    };


    VkFramebufferAttachmentsCreateInfo FramebufferAttachments
    {
      .sType=VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO,
      .attachmentImageInfoCount=1,
      .pAttachmentImageInfos=&FramebufferAttachmentImage,
    };
    
    VkFramebufferCreateInfo framebufferCreateInfo = {};
    framebufferCreateInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    framebufferCreateInfo.pNext                   = &FramebufferAttachments,
    framebufferCreateInfo.flags                   = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT;
    framebufferCreateInfo.renderPass              = renderpass;
    framebufferCreateInfo.width                   = swapChainExtent.width;
    framebufferCreateInfo.height                  = swapChainExtent.height;
    framebufferCreateInfo.layers                  = 1;
    framebufferCreateInfo.attachmentCount         = 1;
    framebufferCreateInfo.pAttachments            = nullptr;



      return Vks::doPointerAlloc5<VkFramebuffer>(&framebufferCreateInfo, vkCreateFramebuffer);
    
  

}

//COuld Turn this into an Dedicated Object or even amore complex "Scene" Aggregate"
auto SwapChain::createRenderPass(VkImageLayout initial, bool load) -> VkRenderPass
{
    std::cout << ( "Creating RenderPass" ) << "\n";
      static const VkAttachmentDescription colorAttachment{
    .format         = VK_FORMAT_B8G8R8A8_UNORM,  // SwapChainSupportDetails::swapChainImageFormat,
    .samples        = VK_SAMPLE_COUNT_1_BIT,
    .loadOp         = VK_ATTACHMENT_LOAD_OP_NONE_EXT,
    .storeOp        = VK_ATTACHMENT_STORE_OP_NONE, //Interestign Bugs: VK_ATTACHMENT_STORE_OP_STORE_DONT_CARE
    .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout  = initial,
    .finalLayout    = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  };
  static constexpr VkAttachmentReference colorAttachmentRef{ .attachment = 0, .layout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL };
  static constexpr VkSubpassDescription  subpass{ .pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                  .inputAttachmentCount = 1,
                                                  .pInputAttachments    = &colorAttachmentRef };

 

  constexpr VkRenderPassCreateInfo vkRenderPassCreateInfo1 = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    //   .flags=VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT,
    .attachmentCount = 1,
    .pAttachments    = &colorAttachment,
    .subpassCount    = 1,
    .pSubpasses      = &subpass,
    // .dependencyCount=1,
    // .pDependencies=&VkSubpassDependency
  };

  return Vks::doPointerAlloc5<VkRenderPass>(&vkRenderPassCreateInfo1, vkCreateRenderPass );

}