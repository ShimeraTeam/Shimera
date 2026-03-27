---
title:          Beta Test Plan
subtitle:       Shimera Library
authors:        Léo Maurel, Paul Arbez and Eddy Gardes
module:         G-EIP-700
version:        1.0
---

<!-- # **BETA TEST PLAN - Shimera** -->

## **1. Project context**
Shimera is a C++ library designed to allow its users (mainly developers) to implement visual effects on top of their graphical projects without using any GPU knowledge. The library provides a set of shaders that can be easily integrated into OpenGL, SFML, and Raylib projects. Each shader comes with a callable function that allows users to implement and modify the shader's parameters according to their needs.

## **2. User Roles**
The following roles will be involved in beta testing:

| **Role Name** | **Description** |
|--------|----------------------|
| Developer | A developer who uses the Shimera library in their own project will test its functionality, ease of use, and stability as a typical end-user. |

## **3. Features Table**
All of the listed features will be demonstrated during the beta presentation

| **Feature ID** | **User role** | **Feature name** | **Short description** |
|--------------|---------------|-------------------------|--------------------------------------|
| F1 | Developer | Choose a graphic library | The user must choose between OpenGL, SFML or Raylib versions of Shimera depending on which library his project uses. |
| F2 | Developer | Add Color Tint shader effect | The user can call a function to add a color tint effect to his project. Color Tint allows the user to edit the color of the screen. |
| F3 | Developer | Configure Color Tint shader effect | The user can configure the color tint effect via its function call. |
| F4 | Developer | Add Distortion shader effect | The user can call a function to add a distortion effect to his project. Distortion allows the user to distort the screen, sort of "liquidify" the screen. |
| F5 | Developer | Configure Distortion shader effect | The user can configure the distortion effect via its function call. |
| F6 | Developer | Add Contrast shader effect | The user can call a function to add a contrast effect to his project. Contrast allows the user to adjust the contrast of the screen's colors. |
| F7 | Developer | Configure Contrast shader effect | The user can configure the contrast strenght via its function call. |
| F8 | Developer | Add Saturation shader effect | The user can call a function to add a saturation effect to his project. Saturation allows the user to adjust the saturation screen's colors. |
| F9 | Developer | Configure Saturation shader effect | The user can configure the saturation strenght via its function call. |
| F10 | Developer | Add Brightness shader effect | The user can call a function to add a brightness effect to his project. Brightness allow the user to adjust the screen's brightness |
| F11 | Developer | Configure Brightness shader effect | The user can configure the brightness strenght via its function call. |
| F12 | Developer | Add Vignette shader effect | The user can call a function to add a vignette effect to his project. Vignette allows the user to add a vignette to the screen. |
| F13 | Developer | Configure Vignette shader effect | The user can configure the vignette effect via its function call. |
| F14 | Developer | Add Chromatic Aberration shader effect | The user can call a function to add a chromatic aberation effect to his project. Chromatic Aberration allows the user to split the red, blue and green shader apart from each other. |
| F15 | Developer | Configure Chromatic Aberration shader effect | The user can configure the chromatic aberration effect via its function call. |
| F16 | Developer | Add Gaussian Blur shader effect | The user can call a function to add a Gaussian blur effect to his project. Gaussian blur allows the user to add a blur to the screen. |
| F17 | Developer | Configure Gaussian Blur shader effect | The user can configure the Gaussian blur effect via its function call. |
| F18 | Developer | Add Pixelation shader effect | The user can call a function to add a pixelation effect to his project. Pixelation allows the user to pixelate the screen. |
| F19 | Developer | Configure Pixelation shader effect | The user can configure the pixelation effect via its function call. |
| F20 | Developer | Add HDR bloom shader effect | The user can call a function to add a HDR bloom effect to his project. HDR bloom allows the user to make surfaces with a certain light amount to glow. |
| F21 | Developer | Configure HDR bloom shader effect | The user can configure the HDR bloom effect via its function call. |
| F22 | Developer | Add Atmospheric Scattering shader effect | The user can call a function to add a atmospheric scattering effect to his project. Atmospheric Scattering adds an atmosphere sphere that react to a light source, changing the colors of the atmosphere, which gives natural effects like sunsets and blue sky durring the day. |
| F23 | Developer | Configure Atmospheric Scattering shader effect | The user can configure the atmospheric scattering effect via its function call. |
| F24 | Developer | Add Fresnel Effect shader | The user can call a function to add a Fresnel Effect to an object. |
| F25 | Developer | Configure Fresnel Effect shader | The user can configure the Fresnel Effect via its function call. |

## **4. Success criteria**
Define the metrics and conditions that determine if the beta version is successful.

| **Feature ID** | **Key success criteria** | **Indicator/metric** | **Result** |
|--------------|---------------------------------------|-----------------------|----------------|
| F1 | A user can use the Shimera library on OpenGL | 2 differents projects, without critical errors | — |
| F2 | A user can use the Shimera library on SFML | 2 differents projects, without critical errors | — |
| F3 | A user can use the Shimera library on Raylib | 2 differents projects, without critical errors | — |
| F4 | A user can implement and configure a shader using a function | 12 shaders linked to its own function | — |
| F5 | A user can use the color shift postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F6 | A user can use the distortion postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F7 | A user can use the contrast postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F8 | A user can use the saturation postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F9 | A user can use brightness postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F10 | A user can use the vignette postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F11 | A user can use the chromatic aberration postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F12 | A user can use the Gaussian blur postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F13 | A user can use the pixelation postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F14 | A user can use HDR bloom postprocessing shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F15 | A user can use the atmospheric scattering shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |
| F16 | A user can use Fresnel Effect spatial shader | VRAM tests or rendering tests or Framerate tests, tested on computer with fixed specifications | — |