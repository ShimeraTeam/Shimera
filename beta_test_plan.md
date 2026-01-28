---
title:          Beta Test Plan
subtitle:       Shimera Library
authors:        Léo Maurel, Paul Arbez and Eddy Gardes
module:         G-EIP-700
version:        1.0
---

<!-- # **BETA TEST PLAN - Shimera** -->

## **1. Project context**
Shimera is a C++ library designed to allow its users (mainly technicians) to implement visual effects on top of their graphical projects without using any GPU knowledge. The library provides a set of shaders that can be easily integrated into OpenGL, SFML, and Raylib projects. Each shader comes with a callable function that allows users to implement and modify the shader's parameters according to their needs.

## **2. User Roles**
The following roles will be involved in beta testing:

| **Role Name** | **Description** |
|--------|----------------------|
| Developer | A developer who uses the Shimera library in their own project will test its functionality, ease of use, and stability as a typical end-user. |

## **3. Features Table**
All of the listed features will be demonstrated during the beta presentation

| **Feature ID** | **User role** | **Feature name** | **Short description** |
|--------------|---------------|-------------------------|--------------------------------------|
| F1 | Developer | Support OpenGL | The library can be used in an OpenGL project. |
| F2 | Developer | Support SFML | The library can be used in an SFML project. |
| F3 | Developer | Support Raylib | The library can be used in a Raylib project. |
| F4 | Developer | Callable function for shader implementation | Each shader will have a callable function that allows you to configure the shader. |
| F5 | Developer | Use colorShift postprocessing shader | Easy-to-implement shader that shifts the screen color. |
| F6 | Developer | Use distortion postprocessing shader | Easy-to-implement shader that distorts the screen. |
| F7 | Developer | Use contrast postprocessing shader | Easy-to-implement shader that changes the screen’s contrast |
| F8 | Developer | Use saturation postprocessing shader | Easy-to-implement shader that saturates the screen. |
| F9 | Developer | Use brightness postprocessing shader | Easy-to-implement shader that brightened the screen. |
| F10 | Developer | Use vignette postprocessing shader | Easy-to-implement shader that adds a vignette to the screen. |
| F11 | Developer | Use chromatic aberration postprocessing shader | Easy-to-implement shader that adds chromatic aberration to the screen. |
| F12 | Developer | Use Gaussian blur postprocessing shader | Easy-to-implement shader that adds Gaussian blur to the screen. |
| F13 | Developer | Use pixelation postprocessing shader | Easy-to-implement shader that pixelates the screen. |
| F14 | Developer | Use HDR bloom postprocessing shader | Easy-to-implement shader that adds bloom to bright objects. |
| F15 | Developer | Use atmospheric scattering postprocessing shader | Easy-to-implement shader that adds atmospheric scattering to a position in the world. |
| F16 | Developer | Use Fresnel Effect spatial shader | Easy-to-implement spatial shader that implements the Fresnel effect on an object. |

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