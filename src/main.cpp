

#include "renderer2.hpp"
#include "fakeFBO.hpp"
#include "SwapChain.hpp"
#include "Pipeline2.hpp"
#include "memSys.hpp"
#include "Win.hpp"
#include "tst2.hpp"


#include <cstdint>
#include <iostream>
#include <pthread.h>
#include "mat4x.hpp"
#include <array>
#include <cstdio>
#include <initializer_list>
#include <iostream>

#include <type_traits>
#include <unistd.h>
#include <vulkan/vulkan_core.h>
#include <windef.h>
#include <winnt.h>
#include <winuser.h>

inline namespace
{
 
  pthread_t sys;
  bool a = true;
   uint32_t aa = 0;
   uint32_t tmSecs = 0;
  static constexpr uint8_t tmOut = 255;
  // static mat4x m4;
 
}

inline void * Sysm( void * pv_unused )
{
  while ( a )
  {
    std::cout << aa /* <<"--->"<< duration  */ << "\n";
    // std::cout << cc /* <<"--->"<< duration  */ << "\n";
    // m4.loadAligned( BuffersX::data );
    // m4.show();
    aa = 0;
    sleep( 1 );
  }
  return nullptr;
}


[[gnu::pure]] auto WinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) -> int
{
  std::array<float, 16> ax = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  mat4x m4(ax.data());
  mat4x m5(ax.data());

  m4.mul(m5);
  // float a = m4[0];
  // m4[0]=a;

  // printf("%llu, \n", &m4);
  int i=0;
  #pragma nounroll
 for (float a : m4) {
      printf("%f ", a);
      i++;
      if((i&3)==0)
      {
        printf("\n");
      }
}
  printf("%f",m5[0]);
  auto x = m5(0);
  std::cout << x << "\n";


  static const Win win(instance);
  static const   VkInit VKI(instance, win.window);
   Vks::Device=VKI.tst();
  static  SwapChain SW(VKI.physdevice, VKI.surface);
  static const Pipeline2 PX2;
  static const renderer2 R2;

  static  memSys memSysm(VKI.physdevice, VKI.device, VKI.instance);                             
  static  tst2 t2(VKI.TransferQueue, PX2.commandBuffer);
  // t2.copyBufToImg(t2.stagingBuffer, t2.image, {width, height, 1});
  
    // const auto pi2 = PX2.genPipeline({VsMCI3temp, VsMCI4temp}, SW.renderpass, VK_CULL_MODE_BACK_BIT, 1);

    // fakeFBO fFBO
    // {
    //   pi2, 
    //   PX2.commandPool, 
    //   SW.renderpass, 
    //   SW.frameBuffer, 
    //   SW.imageViews,
    //   PX2.vkLayout,
    //   PX2.commandBuffer
    // };
    // auto x = PX2.genCommPool();


    // fFBO.doCommBuffers();
    
    // PX2.genCommBuffers();
    std::cout << "VkInit" <<std::is_standard_layout_v<VkInit> << "\n";
    std::cout << "VkInit" <<std::is_trivially_copyable_v<VkInit> << "\n";
    std::cout << "VkInit" <<std::is_trivially_constructible_v<VkInit> << "\n";

    std::cout << "fakeFBO" <<std::is_standard_layout_v<fakeFBO> << "\n";
    std::cout << "fakeFBO" <<std::is_trivially_copyable_v<fakeFBO> << "\n";
    std::cout << "fakeFBO" <<std::is_trivially_constructible_v<fakeFBO> << "\n";
    std::cout << "fakeFBO" <<std::is_trivially_move_constructible_v<fakeFBO> << "\n";
    std::cout << "fakeFBO" <<std::is_pod_v<fakeFBO> << "\n";
    std::cout << "fakeFBO" <<std::is_move_assignable_v<fakeFBO> << "\n";
    std::cout << "fakeFBO" <<std::is_copy_assignable_v<fakeFBO> << "\n";


    // while(IsWindow(win.window))
    {
      static LPMSG msg;
      static DWORD prevTime;
    //  const fakeFBO SFBO=(tmSecs%10==0) ? fFBO : fFBO1;
        const auto x = clock();
        
        // t2.transitionImageLayout(VKI.GraphicsQueue, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, SW.image[renderer2::currentFrame]);
         t2.vkRecImg(
            SW.image[renderer2::currentFrame], renderer2::currentFrame);
        //  t2.vkRecImg(SW.image[renderer2::currentFrame], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

         vkResetFences(VKI.tst(), 1, &R2.fence[renderer2::currentFrame]);
         R2.drawFrame(VKI, SW, {t2.commandBuffers[renderer2::currentFrame]});
        aa++;
        
          PeekMessageA(msg, win.window, WM_KEYFIRST, WM_MOVING, PM_REMOVE);
          
        if(prevTime+CLOCKS_PER_SEC<clock())
        {
          tmSecs++;
        prevTime=x;
          std::cout << aa << "\n";
          aa=0;
        }
    }

    bool aa;
    std::cin>> aa;
 
}

void renderer2::drawFrame(VkInit const &__restrict__ VKI, SwapChain const &__restrict__ SW, std::initializer_list<VkCommandBuffer> commandBuffer) const
{
  // m4.loadAligned( &m5 );
  // if(IsHungAppWindow(VKI.window))
  // {
  //   std::cout << "HUNG!" << "\n";
  // }
 chkTst(vkAcquireNextImageKHR( VKI.tst(), SW.swapChain, 10000, AvailableSemaphore[currentFrame], nullptr, reinterpret_cast<uint32_t*>(&imgIndx) ));
  
  
constexpr VkPipelineStageFlags t=VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
      const  VkSubmitInfo           info{
              .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
              .waitSemaphoreCount = 1,
              .pWaitSemaphores    = &AvailableSemaphore[currentFrame],
              .pWaitDstStageMask  = &t,
              .commandBufferCount = 1,
              .pCommandBuffers= commandBuffer.begin()
  };
    chkTst(vkQueueSubmit( VKI.GraphicsQueue, 1, &info, fence[currentFrame] ));
  // }
  
    const VkPresentInfoKHR VkPresentInfoKHR1{ .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                                                                    .pWaitSemaphores=&FinishedSemaphore[currentFrame],
                                                                    .swapchainCount = 1,
                                                                    .pSwapchains    = &SW.swapChain,
                                                                    .pImageIndices  = reinterpret_cast<uint32_t*>(&imgIndx),
                                                                    .pResults       = nullptr };


  //  info.pWaitSemaphores = &AvailableSemaphore;

 chkTst(vkQueuePresentKHR( VKI.GraphicsQueue, &VkPresentInfoKHR1 ));

          chkTst(vkWaitForFences(VKI.device, 1, &fence[currentFrame], false, -1));
  currentFrame++;
  currentFrame=(currentFrame++&0x7); //Should be notiably faster than modulus if it isn't optimised out by the compiler]: Also allows for the ability to correctly mask the currentFrameincrement against the maxFrameBuffer/Depth/SwapChainImages to ne efefctviley reset to zero wqithout the need to utilsie modulus at all
 
}
