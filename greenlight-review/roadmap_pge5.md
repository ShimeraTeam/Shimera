# Roadmap PGE5 - Shimera

## July 2026 - Decision point
Right now, Shimera has a separate abstraction layer for each backend (OpenGL, SFML, Raylib). The team is testing whether this can be replaced with a single generic architecture that works across any backend. The result determines which path the project follows for the rest of this development cycle.

- Generic architecture works out -> **Plan A**
- Doesn't pan out -> **Plan B** (keep current backend-specific architecture)

*Risk: the test drags on or gives ambiguous results -> decision date kept firm so the rest of the schedule isn't delayed.*

## Plan A (generic architecture works out)

| Period | Features |
|---|---|
| **August** | Refactor the library on a dedicated branch around the new generic architecture |
| **September** | Integrate [Slang](https://shader-slang.org/) into the workflow and migrate existing GLSL shaders to it |
| **October** | Focus on world-space shaders and materials |
| **November – February** | Consolidate the library and expand the shader catalog |

## Plan B (keep current backend-specific architecture)

| Period | Features |
|---|---|
| **August** | Integrate Slang into the workflow and migrate existing GLSL shaders to it |
| **September** | Implement world-space shaders directly in the current library, if not already done |
| **October – February** | Same consolidation and shader-catalog work as Plan A |

## October - World-space shaders & materials (both plans)
- Fog (linear/exponential/volumetric)
- Water Effect
- PBR
- Refraction / Glass

*Technical challenge: these shaders need more GPU resources and a more mature multi-pass rendering architecture than the simple post-processing effects already in place.*

## November - February - Consolidation & shader catalog (both plans)
Remaining post-processing shaders to catch up on:
- Depth of Field, Sobel Filter, Cel Shading, Motion Blur
- SSAO / SSR (if time allows)
- Face Culling / Frustum Culling

### Prioritization if behind schedule
1. Core library + shaders already in place (non-negotiable)
2. World-space / material shaders (October)
3. Advanced shaders (PBR, SSAO/SSR, volumetric fog)
4. Metal / BGFX (first to be dropped if needed)