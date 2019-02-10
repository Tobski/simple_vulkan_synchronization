// Copyright (c) 2017-2019 Tobias Hector

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <vulkan/vulkan.h>
#include <stdio.h>

#define THSVS_SIMPLER_VULKAN_SYNCHRONIZATION_IMPLEMENTATION
#include "../thsvs_simpler_vulkan_synchronization.h"

void global_barrier_test_array(const char* testName,
                               unsigned int numPrevAccesses,
                               ThsvsAccessType* prevAccesses,
                               unsigned int numNextAccesses,
                               ThsvsAccessType* nextAccesses,
                               VkPipelineStageFlags expectedSrcStageMask,
                               VkPipelineStageFlags expectedDstStageMask,
                               VkAccessFlags expectedSrcAccessMask,
                               VkAccessFlags expectedDstAccessMask)
{
    ThsvsGlobalBarrier barrier = {numPrevAccesses, prevAccesses, numNextAccesses, nextAccesses};
    
    VkMemoryBarrier vkBarrier = { 0 };
    VkPipelineStageFlags srcStages = 0;
    VkPipelineStageFlags dstStages = 0;
    unsigned int testPassed = 1;
    
    thsvsGetVulkanMemoryBarrier(barrier, &srcStages, &dstStages, &vkBarrier);

    printf("Test: %s\n", testName);
    
    if (srcStages != expectedSrcStageMask)
    {
        printf("\tUnexpected source stage %0#10X\n", srcStages);
        testPassed = 0;
    }

    if (dstStages != expectedDstStageMask)
    {
        printf("\tUnexpected destination stage %0#10X\n", dstStages);
        testPassed = 0;
    }
    
    if (vkBarrier.srcAccessMask != expectedSrcAccessMask)
    {
        printf("\tUnexpected source access mask %0#10X\n", vkBarrier.srcAccessMask);
        testPassed = 0;
    }
    
    if (vkBarrier.dstAccessMask != expectedDstAccessMask)
    {
        printf("\tUnexpected destination access mask %0#10X\n", vkBarrier.dstAccessMask);
        testPassed = 0;
    }
    
    if (testPassed == 1)
        printf("\tPASSED\n");
    else
        printf("\tFAILED\n");
}

void global_barrier_test(const char* testName,
                         ThsvsAccessType prevAccess,
                         ThsvsAccessType nextAccess,
                         VkPipelineStageFlags expectedSrcStageMask,
                         VkPipelineStageFlags expectedDstStageMask,
                         VkAccessFlags expectedSrcAccessMask,
                         VkAccessFlags expectedDstAccessMask)
{
    global_barrier_test_array(testName, 1, &prevAccess, 1, &nextAccess, expectedSrcStageMask, expectedDstStageMask, expectedSrcAccessMask, expectedDstAccessMask);
}

void image_barrier_test_array(const char* testName,
                              unsigned int numPrevAccesses,
                              ThsvsAccessType* prevAccesses,
                              unsigned int numNextAccesses,
                              ThsvsAccessType* nextAccesses,
                              VkPipelineStageFlags expectedSrcStageMask,
                              VkPipelineStageFlags expectedDstStageMask,
                              VkAccessFlags expectedSrcAccessMask,
                              VkAccessFlags expectedDstAccessMask,
                              VkImageLayout expectedOldLayout,
                              VkImageLayout expectedNewLayout)
{
    ThsvsImageBarrier barrier = {numPrevAccesses, prevAccesses, numNextAccesses, nextAccesses};
    
    VkImageMemoryBarrier vkBarrier = { 0 };
    VkPipelineStageFlags srcStages = 0;
    VkPipelineStageFlags dstStages = 0;
    unsigned int testPassed = 1;
    
    thsvsGetVulkanImageMemoryBarrier(barrier, &srcStages, &dstStages, &vkBarrier);

    printf("Test: %s\n", testName);
    
    if (srcStages != expectedSrcStageMask)
    {
        printf("\tUnexpected source stage %0#10X\n", srcStages);
        testPassed = 0;
    }

    if (dstStages != expectedDstStageMask)
    {
        printf("\tUnexpected destination stage %0#10X\n", dstStages);
        testPassed = 0;
    }
    
    if (vkBarrier.srcAccessMask != expectedSrcAccessMask)
    {
        printf("\tUnexpected source access mask %0#10X\n", vkBarrier.srcAccessMask);
        testPassed = 0;
    }
    
    if (vkBarrier.dstAccessMask != expectedDstAccessMask)
    {
        printf("\tUnexpected destination access mask %0#10X\n", vkBarrier.dstAccessMask);
        testPassed = 0;
    }
    
    if (vkBarrier.oldLayout != expectedOldLayout)
    {
        printf("\tUnexpected old layout %d\n", vkBarrier.oldLayout);
        testPassed = 0;
    }
    
    if (vkBarrier.newLayout != expectedNewLayout)
    {
        printf("\tUnexpected new layout %d\n", vkBarrier.newLayout);
        testPassed = 0;
    }
    
    if (testPassed == 1)
        printf("\tPASSED\n");
    else
        printf("\tFAILED\n");
}

void image_barrier_test(const char* testName,
                        ThsvsAccessType prevAccess,
                        ThsvsAccessType nextAccess,
                        VkPipelineStageFlags expectedSrcStageMask,
                        VkPipelineStageFlags expectedDstStageMask,
                        VkAccessFlags expectedSrcAccessMask,
                        VkAccessFlags expectedDstAccessMask,
                        VkImageLayout expectedOldLayout,
                        VkImageLayout expectedNewLayout)
{
    image_barrier_test_array(testName, 1, &prevAccess, 1, &nextAccess, expectedSrcStageMask, expectedDstStageMask, expectedSrcAccessMask, expectedDstAccessMask, expectedOldLayout, expectedNewLayout);
}

int main(int argc, char* argv[])
{
    global_barrier_test("Compute write to storage buffer/image, Compute read from storage buffer/image",
                        THSVS_ACCESS_COMPUTE_SHADER_WRITE,
                        THSVS_ACCESS_COMPUTE_SHADER_READ_OTHER,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_ACCESS_SHADER_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT);

    global_barrier_test("Compute read from storage buffer, Compute write from storage buffer",
                        THSVS_ACCESS_COMPUTE_SHADER_READ_OTHER,
                        THSVS_ACCESS_COMPUTE_SHADER_WRITE,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        0,
                        0);

    global_barrier_test("Compute write to storage buffer, Graphics read as index buffer",
                        THSVS_ACCESS_COMPUTE_SHADER_WRITE,                        
                        THSVS_ACCESS_INDEX_BUFFER,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                        VK_ACCESS_SHADER_WRITE_BIT,
                        VK_ACCESS_INDEX_READ_BIT);

    {
        ThsvsAccessType prevAccesses[] = {THSVS_ACCESS_COMPUTE_SHADER_WRITE};
        ThsvsAccessType nextAccesses[] = {THSVS_ACCESS_INDEX_BUFFER, THSVS_ACCESS_COMPUTE_SHADER_READ_UNIFORM_BUFFER};
        global_barrier_test_array("Compute write to storage buffer, Graphics read as index buffer & Compute read as uniform buffer",
                                  1, prevAccesses,
                                  2, nextAccesses,
                                  VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                  VK_PIPELINE_STAGE_VERTEX_INPUT_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                  VK_ACCESS_SHADER_WRITE_BIT,
                                  VK_ACCESS_INDEX_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT);
    }
    
    global_barrier_test("Compute write to storage buffer, Graphics read as indirect buffer",
                        THSVS_ACCESS_COMPUTE_SHADER_WRITE,                        
                        THSVS_ACCESS_INDIRECT_BUFFER,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
                        VK_ACCESS_SHADER_WRITE_BIT,
                        VK_ACCESS_INDIRECT_COMMAND_READ_BIT);

    image_barrier_test("Compute write to storage image, Graphics fragment read as sampled image",
                        THSVS_ACCESS_COMPUTE_SHADER_WRITE,                        
                        THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_ACCESS_SHADER_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT,
                        VK_IMAGE_LAYOUT_GENERAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    {
        ThsvsAccessType prevAccesses[] = {THSVS_ACCESS_COMPUTE_SHADER_WRITE};
        ThsvsAccessType nextAccesses[] = {THSVS_ACCESS_INDIRECT_BUFFER, THSVS_ACCESS_FRAGMENT_SHADER_READ_UNIFORM_BUFFER};
        global_barrier_test_array("Compute write to storage texel buffer, Graphics read as indirect buffer & fragment read as uniform buffer",
                            1, prevAccesses,                        
                            2, nextAccesses,
                            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                            VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                            VK_ACCESS_SHADER_WRITE_BIT,
                            VK_ACCESS_INDIRECT_COMMAND_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT);
    }

    image_barrier_test("Graphics write to color attachment, Compute read from sampled image",
                        THSVS_ACCESS_COLOR_ATTACHMENT_WRITE,                        
                        THSVS_ACCESS_COMPUTE_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER,
                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    image_barrier_test("Graphics write to depth attachment, Compute read from sampled image",
                        THSVS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE,                        
                        THSVS_ACCESS_COMPUTE_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER,
                        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    image_barrier_test("Graphics write to depth attachment, Graphics fragment read from input attachment",
                        THSVS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE,                        
                        THSVS_ACCESS_FRAGMENT_SHADER_READ_DEPTH_STENCIL_INPUT_ATTACHMENT,
                        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                        VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);

    image_barrier_test("Graphics write to depth attachment, Graphics fragment read from sampled image",
                        THSVS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE,                        
                        THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER,
                        VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT,
                        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    image_barrier_test("Graphics write to color attachment, Graphics fragment read from input attachment",
                        THSVS_ACCESS_COLOR_ATTACHMENT_WRITE,                        
                        THSVS_ACCESS_FRAGMENT_SHADER_READ_COLOR_INPUT_ATTACHMENT,
                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                        VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    image_barrier_test("Graphics write to color attachment, Graphics fragment read from sampled image",
                        THSVS_ACCESS_COLOR_ATTACHMENT_WRITE,                        
                        THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER,
                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    image_barrier_test("Graphics write to color attachment, Graphics vertex read from sampled image",
                        THSVS_ACCESS_COLOR_ATTACHMENT_WRITE,                        
                        THSVS_ACCESS_VERTEX_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER,
                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
                        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    image_barrier_test("Graphics fragment read from sampled image, Graphics write to color attachment",
                        THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER,                        
                        THSVS_ACCESS_COLOR_ATTACHMENT_WRITE,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        0,
                        0,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    global_barrier_test("None, Transfer read from buffer",
                        THSVS_ACCESS_NONE,                        
                        THSVS_ACCESS_TRANSFER_READ,
                        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                        VK_PIPELINE_STAGE_TRANSFER_BIT,
                        0,
                        0);

    global_barrier_test("Transfer write to buffer, Graphics read from vertex buffer",
                        THSVS_ACCESS_TRANSFER_WRITE,                        
                        THSVS_ACCESS_VERTEX_BUFFER,
                        VK_PIPELINE_STAGE_TRANSFER_BIT,
                        VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
                        VK_ACCESS_TRANSFER_WRITE_BIT,
                        VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT);

    image_barrier_test("Transfer write to image, Graphics fragment read from sampled image",
                        THSVS_ACCESS_TRANSFER_WRITE,                        
                        THSVS_ACCESS_FRAGMENT_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER,
                        VK_PIPELINE_STAGE_TRANSFER_BIT,
                        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                        VK_ACCESS_TRANSFER_WRITE_BIT,
                        VK_ACCESS_SHADER_READ_BIT,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    image_barrier_test("Graphics color attachment write, Presentation",
                        THSVS_ACCESS_COLOR_ATTACHMENT_WRITE,                        
                        THSVS_ACCESS_PRESENT,
                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                        0,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    global_barrier_test("Full pipeline barrier",
                        THSVS_ACCESS_GENERAL,                        
                        THSVS_ACCESS_GENERAL,
                        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                        VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT,
                        VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);
}
