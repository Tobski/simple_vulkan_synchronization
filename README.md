# Simplified Vulkan Synchronization

In an effort to make Vulkan synchronization more accessible, I created this
stb-inspired single-header library in order to somewhat simplify the core
synchronization mechanisms in Vulkan - pipeline barriers and events.

Rather than the complex maze of enums and bitflags in Vulkan - many
combinations of which are invalid or nonsensical - this library collapses
this to a much shorter list of 40 distinct usage types, and a couple of
options for handling image layouts.

Use of other synchonization mechanisms such as semaphores, fences and render
passes are not addressed in this API at present.

## Usage

#define the symbol THSVS_SIMPLER_VULKAN_SYNCHRONIZATION_IMPLEMENTATION in
*one* C/C++ file before the #include of the header; the implementation
will be generated in that file.

## Version

alpha.7

Alpha.7 incorporates a number of fixes from @gwihlidal, and fixes
handling of pipeline stages in the presence of multiple access types or
barriers in light of other recent changes.

## Version History

alpha.6

Alpha.6 fixes a typo (VK_ACCESS_TYPE_MEMORY_READ|WRITE_BIT should have been VK_ACCESS_MEMORY_READ|WRITE_BIT), and sets the pipeline stage src and dst flag bits to VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT and VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT during initialization, not 0 as per alpha.5

alpha.5

Alpha.5 now correctly zeroes out the pipeline stage flags before trying to incrementally set bits on them... common theme here, whoops.

alpha.4

Alpha.4 now correctly zeroes out the access types before trying to incrementally set bits on them (!)

alpha.3
    
Alpha.3 changes the following:
	
Uniform and vertex buffer access in one enum, matching D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER:
 - THSVS_ACCESS_ANY_SHADER_READ_UNIFORM_BUFFER_OR_VERTEX_BUFFER
	 
Color read *and* write access, matching D3D12_RESOURCE_STATE_RENDER_TARGET:
 - THSVS_ACCESS_COLOR_ATTACHMENT_READ_WRITE
	 
Also the "THSVS_ACCESS_\*\_SHADER_READ_SAMPLED_IMAGE" enums have been renamed to the form "THSVS_ACCESS_\*\_SHADER_READ_SAMPLED_IMAGE_OR_UNIFORM_TEXEL_BUFFER"

alpha.2

Alpha.2 adds four new resource states for "ANY SHADER ACCESS":
 - THSVS_ACCESS_ANY_SHADER_READ_UNIFORM_BUFFER
 - THSVS_ACCESS_ANY_SHADER_READ_SAMPLED_IMAGE
 - THSVS_ACCESS_ANY_SHADER_READ_OTHER
 - THSVS_ACCESS_ANY_SHADER_WRITE
 
alpha.1

Alpha.1 adds three new resource states:
 - THSVS_ACCESS_GENERAL (Any access on the device)
 - THSVS_ACCESS_DEPTH_ATTACHMENT_WRITE_STENCIL_READ_ONLY (Write access to only the depth aspect of a depth/stencil attachment)
 - THSVS_ACCESS_STENCIL_ATTACHMENT_WRITE_DEPTH_READ_ONLY (Write access to only the stencil aspect of a depth/stencil attachment)

It also fixes a couple of typos, and adds clarification as to when extensions need to be enabled to use a feature.

alpha.0

This is the very first public release of this library; future revisions
of this API may change the API in an incompatible manner as feedback is
received.
Once the version becomes stable, incompatible changes will only be made
to major revisions of the API - minor revisions will only contain
bugfixes or minor additions.

## Memory Allocation

The thsvsCmdPipelineBarrier and thsvsCmdWaitEvents commands allocate
temporary storage for the Vulkan barrier equivalents in order to pass them
to the respective Vulkan commands.

These use the `temp_alloc(size)` and `temp_free(x)` macros, which are by
default set to `alloca(size)` and nothing, respectively.
If you don't want to use stack space or would rather use your own
allocation strategy, these can be overridden.

I'd rather avoid the need for these allocations in what are likely to be
high-traffic commands, but currently just want to ship something - may
revisit this at a future date based on feedback.

## Expressiveness Compared to Raw Vulkan

Despite the fact that this API is fairly simple, it expresses 99% of
what you'd actually ever want to do in practice.
Adding the missing expressiveness would result in increased complexity
which didn't seem worth the tradeoff - however I would consider adding
something for them in future if it becomes an issue.

Here's a list of known things you can't express:

* Execution only dependencies cannot be expressed.
  These are occasionally useful in conjunction with semaphores, or when
  trying to be clever with scheduling - but their usage is both limited
  and fairly tricky to get right anyway.
* Depth/Stencil Input Attachments can be read in a shader using either
  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL or
  VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL - this library
  *always* uses VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL.
  It's possible (though highly unlikely) when aliasing images that this
  results in unnecessary transitions.

## Error Checks

By default, as with the Vulkan API, this library does NOT check for
errors.
However, a number of optional error checks (`THSVS_ERROR_CHECK_*`) can be
enabled by uncommenting the relevant #defines.
Currently, error checks simply assert at the point a failure is detected
and do not output an error message.
I certainly do not claim they capture *all* possible errors, but they
capture what should be some of the more common ones.
Use of the Vulkan Validation Layers in tandem with this library is
strongly recommended:
    https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers

## Issues

This header was clean of warnings using -Wall as of time of publishing
on both gcc 4.8.4 and clang 3.5, using the c99 standard.

There's a potential pitfall in thsvsCmdPipelineBarrier and thsvsCmdWaitEvents
where alloca is used for temporary allocations. See
[Memory Allocation](#memory-allocation) for more information.

Testing of this library is so far extremely limited with no immediate
plans to add to that - so there's bound to be some amount of bugs.
Please raise these issues on the repo issue tracker, or provide a fix
via a pull request yourself if you're so inclined.
