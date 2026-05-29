# Changelog

> This project is currently in **pre-beta**. No stable version has been released yet. The first official version tag will be assigned at the end of the beta phase.
> 
> This changelog follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) conventions with [Conventional Commits](https://www.conventionalcommits.org/).

## [1.0.0-beta] *(Scheduled for July 2026)*

---

## Tests and Benchmarks *(Apr 2026)*

### Added

- Tests workflow with dedicated GPU runner ([9d097d5](https://github.com/ShimeraTeam/Shimera/commit/9d097d59ac537e98b06c58462466c5b36ce71e77))
- Basic VRAM, FPS and memory benchmark tests ([4a781e9](https://github.com/ShimeraTeam/Shimera/commit/4a781e949afaf0f99e08425969b892e53449c20a))

---

## OpenGL Backend & Full Effect Suite *(Mar - Apr 2026)*

### Added

**OpenGL Backend**

- Full OpenGL backend with framebuffer and texture management ([cdf342d](https://github.com/ShimeraTeam/Shimera/commit/cdf342da30c8a9db596e7a56195e0af47619020e))
- Shader management layer in the OpenGL backend ([0b63d05](https://github.com/ShimeraTeam/Shimera/commit/0b63d05863c2e5c8a38a783b2ed7239b2870aa73))
- OpenGL post-processor ([0db1045](https://github.com/ShimeraTeam/Shimera/commit/0db1045b86fdec20ff73f4fe8a06e680171e1e35))
- `OpenGLTexture` class for texture management ([82faa5b](https://github.com/ShimeraTeam/Shimera/commit/82faa5b55955e4e29d7cc8ad6e22e92f087f9173))
- Distortion and backend effects integrated into the OpenGL example ([2b6618a](https://github.com/ShimeraTeam/Shimera/commit/2b6618ad4567430280e8ca4f38a2c22b548bf3af))

**Effects**

- `VignetteEffect` class and fragment shader ([ac409c7](https://github.com/ShimeraTeam/Shimera/commit/ac409c70ecdb1dc816b09667ee9f9eb4f4ffb666) / [390f752](https://github.com/ShimeraTeam/Shimera/commit/390f7522ce1d49514bb23aa4257bf94736ef1095))
- `GrayscaleEffect` class ([fa38e2d](https://github.com/ShimeraTeam/Shimera/commit/fa38e2d71c664baf1c54c2492cc0319894ff93b1))
- `ChromaticAberrationEffect` with adjustable strength, contrast and sample count ([62a230c](https://github.com/ShimeraTeam/Shimera/commit/62a230c530631f020b4d40ee4c19f45491218c8e))
- `ContrastEffect` class and shader ([efd2f4f](https://github.com/ShimeraTeam/Shimera/commit/efd2f4fee52fc21a550e12dbccd2d355adbf96c3))
- `BrightnessEffect` class and shader ([db55b0c](https://github.com/ShimeraTeam/Shimera/commit/db55b0cf383fe3224c9da3a5382793d35516cbbd))
- `SaturationEffect` class and `.frag` shader ([4111d07](https://github.com/ShimeraTeam/Shimera/commit/4111d07443a4b2e2ce79ddb845ef28d01ea3f684) / [757ea97](https://github.com/ShimeraTeam/Shimera/commit/757ea97b0fb205f1ad6996b3b2ac853e5f303702))
- `ColorShiftEffect` class with `vec3` uniform template ([d3ff2a0](https://github.com/ShimeraTeam/Shimera/commit/d3ff2a0c8a59f0d256dbd746f78312814d5a2b07) / [808fe7e](https://github.com/ShimeraTeam/Shimera/commit/808fe7e2e44a96abac7189df3c0c201ad5b22e24))

**Build**

- Upgraded C++ standard from C++17 to C++23 ([f6ece6d](https://github.com/ShimeraTeam/Shimera/commit/f6ece6da40b264c068d6ec263c5940aed2b2fb6b))
- DLL export/import macros (`SHIMERA_API`) for Windows ([df96d08](https://github.com/ShimeraTeam/Shimera/commit/df96d08dae9e92b1b9f67c9bbb8ac34aad19969e))
- `ChromaticAberrationEffect` exposed for DLL export ([30961b3](https://github.com/ShimeraTeam/Shimera/commit/30961b38d47a68cdf972d8d9200295f724ccc40d))

### Fixed

- SFML crash on window close (window active guard) ([7bcf2e7](https://github.com/ShimeraTeam/Shimera/commit/7bcf2e7b3821cf6738f54478931d3c6bd2df53fd))
- Missing includes causing GCC compilation errors on Linux ([80937c9](https://github.com/ShimeraTeam/Shimera/commit/80937c98dc6fbe8c56d4af9baa4f957846e8c421))
- Hardcoded value in the colorshift shader ([dc75ca1](https://github.com/ShimeraTeam/Shimera/commit/dc75ca1447891a35924b210d6fa2a5394ae1e932))
- Uniform naming unified to `u_strength` across Brightness and Saturation ([fbbf9dd](https://github.com/ShimeraTeam/Shimera/commit/fbbf9dd48b0f6fc6d42a506fd02238f7ef06b5eb))
- Raylib camera initialization made Windows-compatible ([67675fc](https://github.com/ShimeraTeam/Shimera/commit/67675fc019917c06506681cf0152391f15e5b78f))
- SFML/Linux platform build compatibility ([44b8cd6](https://github.com/ShimeraTeam/Shimera/commit/44b8cd67f57d0ac1c4ebb739de5c20d69358880f))

---

## Multi-Backend Abstraction *(Feb - Mar 2026)*

### Added

- Multi-backend abstraction layer with SFML integration and multi-pass post-processing support ([2bab6e0](https://github.com/ShimeraTeam/Shimera/commit/2bab6e09285bac9e70ff16d6dea0bec67fef2633))
- Raylib backend integration ([86e2c81](https://github.com/ShimeraTeam/Shimera/commit/86e2c819a9d1e27674c91e7b9d132da4aed902c4))
- Raylib backend CI validation workflow ([66fcc78](https://github.com/ShimeraTeam/Shimera/commit/66fcc78c60eac75e25675de3602c4eee357d3e6b))
- `DistortionEffect` class and shader abstraction ([ec25c73](https://github.com/ShimeraTeam/Shimera/commit/ec25c73f4e7cd6170ec91983a2cf4a6851a0c166))
- Brightness fragment shader ([ed63e0d](https://github.com/ShimeraTeam/Shimera/commit/ed63e0d5115409863a1a5e5598e30bb8b7f8eecd))
- Contrast and normal fragment shaders ([3fa3b75](https://github.com/ShimeraTeam/Shimera/commit/3fa3b75d14ccea794b0597b5f137930cc559f220))

### Fixed

- Build workflow refactored and deprecated pipeline removed ([a34d362](https://github.com/ShimeraTeam/Shimera/commit/a34d362f5a31c3d44ec828d502b1bd47c0374acf))

---

## CI/CD, Deploy and First Shaders *(Jan - Feb 2026)*

### Added

- Automated release and pre-release workflow with versioned GitHub releases ([b615070](https://github.com/ShimeraTeam/Shimera/commit/b615070f307ac9307fdad2f75ce196127967e9b1))
- Shader rendering in Raylib example ([fe0199d](https://github.com/ShimeraTeam/Shimera/commit/fe0199d12c331663cb27ce706fe8ca5562d7ed04))
- Raylib example project ([c53049f](https://github.com/ShimeraTeam/Shimera/commit/c53049f8f84cd0795882b4781a5a004f822bca2d))
- Git hooks with `commit-msg` enforcement ([345a95d](https://github.com/ShimeraTeam/Shimera/commit/345a95dec525e927b71d79b79e3d1cd4c0b0ba03))
- CI restricted to `dev` and `master` branches ([34f0b48](https://github.com/ShimeraTeam/Shimera/commit/34f0b48775e8e15b52017a9d4548b021158eca95))
- Shimera shared library binary compilation and xmake integration ([d3e5686](https://github.com/ShimeraTeam/Shimera/commit/d3e5686f8a9f7719ca7738ffa8438ed7b1e96b77))
- Documentation submodule linked to ShimeraDoc repo ([708d595](https://github.com/ShimeraTeam/Shimera/commit/708d5952ce332118a20f8ba1e22e3f02470fe26a))
- Beta test plan ([17d41f0](https://github.com/ShimeraTeam/Shimera/commit/17d41f09f8b44fa530b502f2cad1eefe5728161c))

---

## Example Projects and Project Bootstrap *(Oct - Nov 2025)*

### Added

- SFML example project ([7a3e798](https://github.com/ShimeraTeam/Shimera/commit/7a3e798db1a7b3c726f06eeacebc4765e8604a19))
- SDL example project ([1b6b70b](https://github.com/ShimeraTeam/Shimera/commit/1b6b70b768e46f46022b0cec2c06fc7faafeeedb))
- OpenGL example project with framebuffer, post-processing quad, and shaders (grayscale, colorshift, distortion) ([595585f](https://github.com/ShimeraTeam/Shimera/commit/595585f840e62359477e0dc01dedeefa73e81bd2))
- Learnings submodules ([b1d0db0](https://github.com/ShimeraTeam/Shimera/commit/b1d0db0b250b2db584a1dd8a22445b6dbcd370f1))
- SFML + OpenGL post-processing integration ([a02d53f](https://github.com/ShimeraTeam/Shimera/commit/a02d53febd82ee0d86732e2f421d90ba9a2d9928))
- Shimera accessibility file ([58dd87d](https://github.com/ShimeraTeam/Shimera/commit/58dd87d628957214a8c2c6457ea76b1b064b9cbd))

---

## Project Bootstrap *(Jun 2025)*

### Added

- Project structure, deployment docs, technical environment and testing policy ([be251f8](https://github.com/ShimeraTeam/Shimera/commit/be251f8bc1b0a1057345e88599f8f9f00c64ae8a))
- GitHub Actions mirror workflow to Epitech ([45e1b45](https://github.com/ShimeraTeam/Shimera/commit/45e1b45660567a29c53374763812eec036d62221))

---

> Version tags will be introduced starting from the **first beta release**.
> The history above represents the full pre-beta development of Shimera.