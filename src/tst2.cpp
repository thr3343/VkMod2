
#include "tst2.hpp"
#include "memSys.hpp"
#include "imgLud.hpp"
#include <cstddef>
#include <cstdint>


// VmaAllocationCreateInfo VmaAllocationCreateInfo1 //DUPE
//   {
//     .usage=VMA_MEMORY_USAGE_AUTO,
//     .memoryTypeBits=VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    
//   };
//This Method/Function Sucks [Mostly/Primarily In terms of Layout]
VkImage tst2::setupImagetest()
{
  auto a = clock();
    int x, y, cnls;
        FILE *f = fopen64(R"(tst.png)", "rb");
        printf("%s %ld \n", "Opened Image in :", clock() - a/CLOCKS_PER_SEC);
    auto limg = tstA::loadImg(*f, x, y, cnls);
    std::cout << sizeof(limg) <<"--"<< x <<"--"<< y <<"--"<< cnls << "\n";
    printf("%s %ld \n", "Loaded Image in :", clock() - a/CLOCKS_PER_SEC);
 
	  xi=x;
	  yi=y;
    VkDeviceSize imageSize = xi * yi * 4;


    Offs = VkOffset2D{x, y};

    stagingBuffer = memSys::allocBuf(imageSize, VmaAllocation);
    //memSys::addMappedSection(VmaAllocation, imageSize);
    chkTst(vmaMapMemory(memSys::vmaAllocator, VmaAllocation, (&memSys::data)));
    {

	    memcpy(memSys::data, limg, imageSize);

    }
    vmaUnmapMemory(memSys::vmaAllocator, VmaAllocation);
        printf("%s %ld \n", "Copied Image in :", clock() - a/CLOCKS_PER_SEC);

	  // memSys::mapMem(limg, imageSize);

	// tstA::freeImg(limg);

   VkImageCreateInfo imageInfo 
   {
        .sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType         = VK_IMAGE_TYPE_2D,
        .format            = VK_FORMAT_R8G8B8A8_UNORM, //Check if correct format as Framebuffer/ViImage.VkImageView is in UNORM and not SRGB
        .extent            = {.width=xi, .height=yi, .depth=1},
        .mipLevels         = 1,
        .arrayLayers       = 1,
        .samples           = VK_SAMPLE_COUNT_1_BIT,
        .tiling            = VK_IMAGE_TILING_OPTIMAL,
        .usage             = srcBit,
        .sharingMode       = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout     = VK_IMAGE_LAYOUT_PREINITIALIZED,//VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,: Check this correct Image Layout
  };
	constexpr VmaAllocationCreateInfo dimg_allocinfo = 
  {
    .flags=VMA_ALLOCATION_CREATE_CAN_ALIAS_BIT,
    .usage=VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
    .memoryTypeBits=0,
    .pool=nullptr
  };
  VkImage vkImage;// = Vks::doPointerAlloc5<VkImage>( &imageInfo, vkCreateImage);

    chkTst(vmaCreateImage(memSys::vmaAllocator, &imageInfo, &dimg_allocinfo, &vkImage, &VmaAllocation, nullptr));
    // chkTst(vmaCreateImage(memSys::vmaAllocator, &imageInfo, &dimg_allocinfo, &DstImg, &VmaAllocation, nullptr));
        // chkTst(vmaAllocateMemoryForImage(memSys::vmaAllocator, vkImage, &dimg_allocinfo, &VmaAllocation, nullptr));
              printf("%s %ld \n", "Created Image in :", clock() - a/CLOCKS_PER_SEC);
  
        // assert(vkImage);
    VkCommSet commandBufferSets(1);
    VkBufferImageCopy copyRegion
    {
	    .bufferOffset = 0,

	    .imageSubresource=subresource,
	    .imageExtent = {xi, yi, 1},
    };

  printf("Allocating img:");
	//copy the buffer into the image
  commandBufferSets.beginSingleTimeCommands();
  {
    vkCmdCopyBufferToImage(commandBufferSets.commandBuffer, stagingBuffer, vkImage, dstLayout, 1, &copyRegion);
    transitionImageLayout(commandBufferSets.commandBuffer, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkImage);
  }
  commandBufferSets.endSingleTimeCommands(TransferQueue2);
        printf("%s %ld \n", "Copied Buff-ToImg in :", clock() - a/CLOCKS_PER_SEC);

    return vkImage;
}


/* VkImage tst2::createEmptImg()
{
   constexpr VkImageCreateInfo imageInfo 
   {
        .sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType         = VK_IMAGE_TYPE_2D,
        .extent            = {.width=width, .height=height, .depth=1},
        .mipLevels         = 1,
        .arrayLayers       = 1,
        .format            = VK_FORMAT_R8G8B8A8_UNORM, //Check if correct format as Framebuffer/ViImage.VkImageView is in UNORM and not SRGB
        .tiling            = VK_IMAGE_TILING_OPTIMAL,
        .initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED,//VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,: Check this correct Image Layout
        .usage             = dstBit,
        .samples           = VK_SAMPLE_COUNT_1_BIT,
        .sharingMode       = VK_SHARING_MODE_EXCLUSIVE,
  };


	constexpr VmaAllocationCreateInfo dimg_allocinfo = {.usage = VMA_MEMORY_USAGE_AUTO};

  VkImage vkImage;// vkImage = Vks::doPointerAlloc5<VkImage>( &imageInfo, vkCreateImage);


    chkTst(vmaCreateImage(memSys::vmaAllocator, &imageInfo, &dimg_allocinfo, &vkImage, &memSys::reemptMappedSection, nullptr));
      VkCommSet commandBufferSets;
    commandBufferSets.beginSingleTimeCommands();

            transitionImageLayout(commandBufferSets.commandBuffer, VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vkImage);

            commandBufferSets.endSingleTimeCommands(GraphicsQueue2);

    return vkImage;
;

} */

void tst2::transitionImageLayout( VkCommandBuffer commandBuffer, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkImage &image)
{

  VkImageMemoryBarrier barrier = {
    .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .pNext               = VK_NULL_HANDLE,
    .oldLayout           = oldLayout,
    .newLayout           = ( newLayout ),
    .srcQueueFamilyIndex = ( VK_IMAGE_LAYOUT_UNDEFINED ),
    .dstQueueFamilyIndex = ( VK_IMAGE_LAYOUT_UNDEFINED ),
    .image               = ( image ),
    .subresourceRange    = { .aspectMask     = static_cast<VkImageAspectFlags>( ( format ) ),
                             .baseMipLevel   = ( 0 ),
                             .levelCount     = ( 1 ),
                             .baseArrayLayer = ( 0 ),
                             .layerCount     = ( 1 ) },
  };

  if ( newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL )
  {
    if ( format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT )
    {
      barrier.subresourceRange.aspectMask = ( barrier.subresourceRange.aspectMask | VK_IMAGE_ASPECT_STENCIL_BIT );
    }
  }

  VkPipelineStageFlags sourceStage      = 0;
  VkPipelineStageFlags destinationStage = 0;
  switch ( oldLayout )
  {
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
      {
        barrier.srcAccessMask = ( VK_ACCESS_TRANSFER_READ_BIT );
        break;
      };
    case VK_IMAGE_LAYOUT_UNDEFINED:
      {
        barrier.srcAccessMask = ( VK_ACCESS_TRANSFER_READ_BIT );
        break;
      };
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
      {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;
      };
    default:  printf( "Unsupported layout transition" ), exit(1);
  }
  switch ( newLayout )
  {
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
      {
        barrier.subresourceRange.aspectMask = ( VK_IMAGE_ASPECT_DEPTH_BIT );

        barrier.dstAccessMask = ( VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT );
        sourceStage           = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage      = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        break;
      }
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
      {
        barrier.subresourceRange.aspectMask = ( VK_IMAGE_ASPECT_COLOR_BIT );

        barrier.dstAccessMask = ( VK_ACCESS_SHADER_READ_BIT );
        sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        break;
      }
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    {
       barrier.subresourceRange.aspectMask = ( VK_IMAGE_ASPECT_COLOR_BIT );

        barrier.dstAccessMask = ( VK_ACCESS_TRANSFER_READ_BIT );
        sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;
    }
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    {
       barrier.subresourceRange.aspectMask = ( VK_IMAGE_ASPECT_COLOR_BIT );

        barrier.dstAccessMask = ( VK_ACCESS_TRANSFER_WRITE_BIT );
        sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;
    }
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
    {
       barrier.subresourceRange.aspectMask = ( VK_IMAGE_ASPECT_COLOR_BIT );

        barrier.dstAccessMask = ( VK_ACCESS_TRANSFER_WRITE_BIT );
        sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;
    }
    default:  printf( "Unsupported layout transition" ), exit(1);
  }

  vkCmdPipelineBarrier( commandBuffer, sourceStage /* TODO */, destinationStage /* TODO */, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier );
}


void tst2::copyImgToBuf(VkImage &img)
{
  VkCommSet commandBufferSets(1);
    VkBufferImageCopy copyRegion = {
	    .bufferOffset = 0,
	    .bufferRowLength = 0,
	    .bufferImageHeight = 0,

	    .imageSubresource=subresource,
	    .imageExtent = {xi, yi, 1},
};
printf("Allocating img:");
	//copy the buffer into the image
  commandBufferSets.beginSingleTimeCommands();
	vkCmdCopyBufferToImage(commandBufferSets.commandBuffer, 
  stagingBuffer, img, dstLayout, 1, &copyRegion);
  commandBufferSets.endSingleTimeCommands(TransferQueue2);
}

[[gnu::pure]] auto constexpr fma2(float a, float b, float c)
{
  return (a*b)+c;
}
[[gnu::pure]] auto consteval tPow(auto a)
{
  return a*a;
}

[[gnu::pure]] auto constexpr dot(VkExtent2D extent)
{
  return extent.height*extent.width;
}

[[gnu::pure]] auto __vectorcall aspect(VkExtent2D extent)
{
  constexpr auto asp =static_cast<double>(width)/height;
  // auto diag = sqrt(__builtin_fma(width, width, tPow(height)));
  // auto h = diag/(sqrt(__builtin_fma(asp, asp, 1)));
  // // printf("%f \n",h);

  // auto x = height*asp/sqrt(__builtin_fma(width, width, tPow(height)));
  
  // // printf("%f \n",x);

  // // auto xy = static_cast<int32_t>(floor(width-sqrt(dot(extent)/asp/(4096/width))))*(1920*1080)/(4096*2048);
  // auto xy = static_cast<int32_t>(floor(width-sqrt(dot(extent)/asp/(4096/width))))/4;

  // auto asp2 = (float)extent.width/extent.height*height;
  auto asp3 = (float)extent.width/extent.height  ;
return (asp3>asp) ? VkOffset2D{(int)(height-(width/asp3)), 0} : VkOffset2D{0, (int)(fmaf(width, asp3, -width))};
  
  // printf("%ld \n", xy);
  // printf("------------------------------------------------------------------------------------------------------------------------------------------------------------>\n");
  // printf("%f \n", asp2);
}

void tst2::vkRecImg(VkImage &img, int a)
{
  auto xyl=aspect({xi, yi});
  int aa = __builtin_fma(width, height, 10);
  
  VkImageBlit imgBlt
  {
    .srcSubresource=subresource,
    .srcOffsets={{0,0,0},{Offs.x, Offs.y, 1}},
    .dstSubresource=subresource,
    .dstOffsets={{-xyl.y,xyl.x,0},{width+(xyl.y>>1),height-(xyl.x>>1),1}},
  };


    constexpr VkRect2D renderArea = { .offset = { 0, 0 }, .extent = {width, height} };


  
  static constexpr VkDeviceSize offsets[] = { 0 };
  
  // /* for(const VkCommandBuffer &commandBuffer : commandBuffers)
  // {*/VkRenderPassAttachmentBeginInfo RenderPassAttachments
  // {
  //   .sType = VK_STRUCTURE_TYPE_RENDER_PASS_ATTACHMENT_BEGIN_INFO,
  //   .attachmentCount = 1,
  //   .pAttachments = &imageViews
  // };

  // const VkRenderPassBeginInfo renderPassInfo = {
  //   .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO ,
  //   .pNext = &RenderPassAttachments,
  //   .renderPass      = renderPass ,
  //   .framebuffer =  frameBuffer ,
  //   .renderArea      = renderArea,
  //   // .clearValueCount = 1,
  //   // .pClearValues    = &clearValues2,
  // };
    constexpr VkCommandBufferBeginInfo beginInfo1 = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                                                    .flags = ( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) };
  vkBeginCommandBuffer(commandBuffers[a], &beginInfo1);

  vkCmdBlitImage(commandBuffers[a], image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, img, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imgBlt, VK_FILTER_LINEAR);
        transitionImageLayout(commandBuffers[a], VK_FORMAT_B8G8R8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, img);
  // vkCmdBeginRenderPass(commandBuffers[a], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  // vkCmdEndRenderPass(commandBuffers[a]);
    vkEndCommandBuffer( commandBuffers[a] );



// return DstImg;

  
}


//ideally would use Buffalloc in function btu would require static struct.lcas. fucntion for MemSys (Which woudl get annying to initalise very Quickely)
auto tst2::copyBufToImg(VkBuffer buffer, VkImage image, VkExtent3D extent)
{
  VkCommSet commandBufferSets(1);
  	const VkBufferImageCopy copyRegion = {
	    .bufferOffset = 0,
	    .bufferRowLength = 0,
	    .bufferImageHeight = 0,

	    .imageSubresource=subresource,
	    .imageExtent = extent,
    };
    
    vkCmdCopyImageToBuffer(commandBufferSets.commandBuffer, image, srcLayout, buffer, 1, &copyRegion);
    return buffer;
}