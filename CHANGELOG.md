# Changelog

> This project is in **beta**. The first official version tag (`0.3.6`) marks the start of the beta phase.
> 
> This changelog follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) conventions with [Conventional Commits](https://www.conventionalcommits.org/).

## Licensing & Open Source *(Jun - Jul 2026)*

### Added

- GNU General Public License v3.0 added to the repository ([029bfd1](https://github.com/ShimeraTeam/Shimera/commit/029bfd1fb14c2f43484067ef07deeeeefc5719ed))
- SPDX license headers added to all source files and shaders ([03a70aa](https://github.com/ShimeraTeam/Shimera/commit/03a70aa76b5b459266b05f1b93b5a63a3fc48d45))
- GPL-3.0 license header added to remaining files ([90c5f09](https://github.com/ShimeraTeam/Shimera/commit/90c5f09f4067648eb78515e7f5a3b72e42cbb466))
- Licensing information and updated contribution guidelines ([55d008e](https://github.com/ShimeraTeam/Shimera/commit/55d008ed94788d3f649f1a98326b32352f2ff6ba) / [87d1d26](https://github.com/ShimeraTeam/Shimera/commit/87d1d2601808ac2988b5a63cf4a90b36a34e75c3))

## Materials, Fresnel & Atmospheric Scattering *(Jun 2026)*

### Added

- Full OpenGL Material support, partial RayLib Material support, and Fresnel Effect Material shader ([cae5ddc](https://github.com/ShimeraTeam/Shimera/commit/cae5ddc3d3a7c701b2a27c1ce273107444db6b53))
- Atmospheric Scattering effect with depth buffer support ([2eda0d6](https://github.com/ShimeraTeam/Shimera/commit/2eda0d6a9ed0ddaf60e543bbc336fbd1a69fa4e1))
- Effect pipeline compatibility with depth texture ([3a1b031](https://github.com/ShimeraTeam/Shimera/commit/3a1b0316b3116a0995a840ab5d1c2be5eced0778))

### Fixed

- `RaylibConvert` empty class removed ([2f74d64](https://github.com/ShimeraTeam/Shimera/commit/2f74d642c986dd62065e44e99c3c0d1a937912fb))
- Duplicate includes and member variable removed from `SFMLPostProcessor` ([39a5131](https://github.com/ShimeraTeam/Shimera/commit/39a5131239d8e4e34360113369359194fb288878))
- Wrong `#ifndef` guards renamed across older files ([f1441a3](https://github.com/ShimeraTeam/Shimera/commit/f1441a3c9231dc011e40373b5ad0ead2d1de7cda))

## HDR Bloom, Gaussian Blur & Pixelation *(May - Jun 2026)*

### Added

- HDR Bloom effect implementation ([b19d256](https://github.com/ShimeraTeam/Shimera/commit/b19d256e9f006496262230a05b87ce686a8cfa0b))
- Gaussian Blur effect and related component updates ([8bb4e6d](https://github.com/ShimeraTeam/Shimera/commit/8bb4e6da95386502f83dec5af40ce26213b058c9))
- Pixelisation effect ([25069d0](https://github.com/ShimeraTeam/Shimera/commit/25069d01e63fccdc47cd6f8922928f10e8bcea70))

### Fixed

- Removed `ACESFilm` function and adjusted bloom color calculation ([7a2783e](https://github.com/ShimeraTeam/Shimera/commit/7a2783e5745fce956b6a4eb6006cce4a3cf2b2fe))
- Blur and HDR bloom temporarily disabled on Raylib backend pending fix ([31b9346](https://github.com/ShimeraTeam/Shimera/commit/31b934624b10f831b7b613d5e9d25bd859f705cb))
- `gaussianBlur` and `HdrBloom` missing declarations in SFML example ([5a2d3d3](https://github.com/ShimeraTeam/Shimera/commit/5a2d3d39f6c1731a9be48b8ae2c3ae54d5522336))

## Effect Pipeline, Shared Builds & Dependencies *(Apr - Jul 2026)*

### Added

- `EffectPipeline` builder added (with example); xmake now includes `.inl` files ([df9a724](https://github.com/ShimeraTeam/Shimera/commit/df9a72464f80809f453623e00e0fef079ba3bf2a))
- Material handling reworked for RayLib and adapted for OpenGL/SFML ([d597f55](https://github.com/ShimeraTeam/Shimera/commit/d597f55930e350742399d0aeeb185885218c6718))

### Fixed

- Class definitions adjusted for shared builds compatibility ([59eb1d0](https://github.com/ShimeraTeam/Shimera/commit/59eb1d0a05cd7ea22e4ab726b7b8bc82bd8c4f94))
- Raylib shared example build fixed ([a928634](https://github.com/ShimeraTeam/Shimera/commit/a9286344604767121e179d25730ee402585bf255))
- SFML shared build fixed ([3f16449](https://github.com/ShimeraTeam/Shimera/commit/3f164497b516bcd1bd4d3cee624572d8cf77c528))
- Missing `glfw` package in xmake ([300979b](https://github.com/ShimeraTeam/Shimera/commit/300979ba897899b5ea348050693eba7cce2f5bbc))
- Dependency installation issues resolved ([55c7ba8](https://github.com/ShimeraTeam/Shimera/commit/55c7ba86f6a94de93701a46c2d3c3233e6a7a062))
- Framebuffer creation validation added ([ff267ac](https://github.com/ShimeraTeam/Shimera/commit/ff267ace0d6b94ed3069f941985d345f1e0aef08))
- Distortion effect updated to use `withTime` for time parameter ([13aa5f8](https://github.com/ShimeraTeam/Shimera/commit/13aa5f8914e1d64ab1b40ff74cd155ae5f49554e))

## Testing Infrastructure Expansion *(Apr - Jul 2026)*

### Added

- Test abstraction layer for effects ([e755fa1](https://github.com/ShimeraTeam/Shimera/commit/e755fa165bba4245aa4872396d805dcd59757ba0))
- Abstraction for multi-effect tests ([11b8985](https://github.com/ShimeraTeam/Shimera/commit/11b898518f61797d9a57a2aeec2f1e85c8c6bd6e))
- Resilience tests, plus shader benchmarks ([db07fb2](https://github.com/ShimeraTeam/Shimera/commit/db07fb24db903a61ad1b13a31cf24287ecbf0f90) / [349bd50](https://github.com/ShimeraTeam/Shimera/commit/349bd50df4530e4718506e4459ee56dd03f18acb))
- Benchmark results export to JSON, wired into CI ([9e337fa](https://github.com/ShimeraTeam/Shimera/commit/9e337fae6a12ddda6f4c385e32a944ac38a9a439) / [9bee5d4](https://github.com/ShimeraTeam/Shimera/commit/9bee5d4f4ef84eb25614f896920408428d3e5355))
- `.clang-tidy` config wired into pre-commit alongside `compile_commands.json` generation ([115b9e4](https://github.com/ShimeraTeam/Shimera/commit/115b9e4e43283a8a9b54ac116a44ecf6e00763e7) / [a5b9c09](https://github.com/ShimeraTeam/Shimera/commit/a5b9c09dd8b0b244280ac348c8c7a93c5b116bd8))

### Fixed

- Missing `shimera` namespace in tests ([b6c55fd](https://github.com/ShimeraTeam/Shimera/commit/b6c55fd11064507607eb609c732f7b7c06d7d100))
- All clang-tidy warnings resolved ([391cc03](https://github.com/ShimeraTeam/Shimera/commit/391cc038f238ece20adad7cc5611c279cb02d084))
- Resilience tests and `getName` effects function fixed ([a0f1896](https://github.com/ShimeraTeam/Shimera/commit/a0f1896b0394ec09d4f1d5d3ca77e050383840b3))
- Missing `virtual` in effects and Raylib tests ([2776b2d](https://github.com/ShimeraTeam/Shimera/commit/2776b2d4486270cdf5906179d338e046f956a20c))
- Windows CI process no longer hangs (force close) ([43873db](https://github.com/ShimeraTeam/Shimera/commit/43873db69699fa59d5b7a18dc3bcf7018164c739))
- vsync disabled for benchmarks; Raylib VRAM measurement fixed ([a8d4cbb](https://github.com/ShimeraTeam/Shimera/commit/a8d4cbbb2ec634471e1cc3c9520e212e057f9e24))

## CI/CD & Release Pipeline *(Apr - Jun 2026)*

### Added

- Submodule auto-update workflow ([06af34f](https://github.com/ShimeraTeam/Shimera/commit/06af34f40ce2669b66bebfe69fa5ab244bdb203e))
- README and changelog now included in build artifacts and release body ([bd18a69](https://github.com/ShimeraTeam/Shimera/commit/bd18a6901b238faf13b39c4cb7b3673a4c7b3979))
- Release/debug variants built with proper artifact packaging ([d800a53](https://github.com/ShimeraTeam/Shimera/commit/d800a534d9151e64869c64fe15d90df733ae1e15))
- Release artifacts split by backend and architecture ([5c9cabb](https://github.com/ShimeraTeam/Shimera/commit/5c9cabb3b566eb4fd0061ece90bc5048c2c7d274))
- Cross-platform commit message validation hook ([820c49f](https://github.com/ShimeraTeam/Shimera/commit/820c49f877785ef4cdaa54299638522b5c7897dd))

### Fixed

- Include folder path fixed in release packaging ([95675af](https://github.com/ShimeraTeam/Shimera/commit/95675af242ced91b4d95a0850327d035fe8dfd9e))

## Documentation *(Apr - Jul 2026)*

### Added

- Curated pre-beta changelog established ([0470651](https://github.com/ShimeraTeam/Shimera/commit/0470651fe2a824a2a416351b6833cb87aecbfb25))
- Contributing guidelines, expanded with additional documentation links ([2d3cfbe](https://github.com/ShimeraTeam/Shimera/commit/2d3cfbe6cd4f896c06e36bab92a471d5f03c8ec0) / [f3f49ff](https://github.com/ShimeraTeam/Shimera/commit/f3f49ff3aeac8cf29958eb66b40cab4feffd27ea))
- README enhanced with detailed project description and example GIF ([3ddece1](https://github.com/ShimeraTeam/Shimera/commit/3ddece140ae1d44b7f5546044b6a787e9bb082ae))

### Fixed

- `AUTHORS.md` updated ([f22e4dd](https://github.com/ShimeraTeam/Shimera/commit/f22e4ddeed24955a4b09ec53b63b019d216f573c))

## Tests and Benchmarks *(Apr 2026)*

### Added

- Tests workflow with dedicated GPU runner ([9d097d5](https://github.com/ShimeraTeam/Shimera/commit/9d097d59ac537e98b06c58462466c5b36ce71e77))
- Basic VRAM, FPS and memory benchmark tests ([4a781e9](https://github.com/ShimeraTeam/Shimera/commit/4a781e949afaf0f99e08425969b892e53449c20a))

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

## Example Projects and Project Bootstrap *(Oct - Nov 2025)*

### Added

- SFML example project ([7a3e798](https://github.com/ShimeraTeam/Shimera/commit/7a3e798db1a7b3c726f06eeacebc4765e8604a19))
- SDL example project ([1b6b70b](https://github.com/ShimeraTeam/Shimera/commit/1b6b70b768e46f46022b0cec2c06fc7faafeeedb))
- OpenGL example project with framebuffer, post-processing quad, and shaders (grayscale, colorshift, distortion) ([595585f](https://github.com/ShimeraTeam/Shimera/commit/595585f840e62359477e0dc01dedeefa73e81bd2))
- Learnings submodules ([b1d0db0](https://github.com/ShimeraTeam/Shimera/commit/b1d0db0b250b2db584a1dd8a22445b6dbcd370f1))
- SFML + OpenGL post-processing integration ([a02d53f](https://github.com/ShimeraTeam/Shimera/commit/a02d53febd82ee0d86732e2f421d90ba9a2d9928))
- Shimera accessibility file ([58dd87d](https://github.com/ShimeraTeam/Shimera/commit/58dd87d628957214a8c2c6457ea76b1b064b9cbd))

## Project Bootstrap *(Jun 2025)*

### Added

- Project structure, deployment docs, technical environment and testing policy ([be251f8](https://github.com/ShimeraTeam/Shimera/commit/be251f8bc1b0a1057345e88599f8f9f00c64ae8a))
- GitHub Actions mirror workflow to Epitech ([45e1b45](https://github.com/ShimeraTeam/Shimera/commit/45e1b45660567a29c53374763812eec036d62221))

---

**0.3.6-beta is the first official beta release of Shimera.** Version tags will follow standard semver going forward.