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
| F1 | Developer | Choose a graphic library | The user must choose between OpenGL, SFML or Raylib versions of Shimera depending on which library their project uses. |
| F2 | Developer | Add Color Tint shader effect | The user can call a function to add a color tint effect to their project. Color Tint allows the user to edit the color of the screen. |
| F3 | Developer | Configure Color Tint shader effect | The user can configure the color tint effect via its function call. |
| F4 | Developer | Add Distortion shader effect | The user can call a function to add a distortion effect to their project. Distortion allows the user to distort the screen, sort of "liquidify" the screen. |
| F5 | Developer | Configure Distortion shader effect | The user can configure the distortion effect via its function call. |
| F6 | Developer | Add Contrast shader effect | The user can call a function to add a contrast effect to their project. Contrast allows the user to adjust the contrast of the screen's colors. |
| F7 | Developer | Configure Contrast shader effect | The user can configure the contrast strength via its function call. |
| F8 | Developer | Add Saturation shader effect | The user can call a function to add a saturation effect to their project. Saturation allows the user to adjust the saturation of the screen's colors. |
| F9 | Developer | Configure Saturation shader effect | The user can configure the saturation strength via its function call. |
| F10 | Developer | Add Brightness shader effect | The user can call a function to add a brightness effect to their project. Brightness allows the user to adjust the screen's brightness |
| F11 | Developer | Configure Brightness shader effect | The user can configure the brightness strength via its function call. |
| F12 | Developer | Add Vignette shader effect | The user can call a function to add a vignette effect to their project. Vignette allows the user to add a vignette to the screen. |
| F13 | Developer | Configure Vignette shader effect | The user can configure the vignette effect via its function call. |
| F14 | Developer | Add Chromatic Aberration shader effect | The user can call a function to add a chromatic aberration effect to their project. Chromatic Aberration allows the user to split the red, blue and green shader apart from each other. |
| F15 | Developer | Configure Chromatic Aberration shader effect | The user can configure the chromatic aberration effect via its function call. |
| F16 | Developer | Add Gaussian Blur shader effect | The user can call a function to add a Gaussian blur effect to their project. Gaussian blur allows the user to add a blur to the screen. |
| F17 | Developer | Configure Gaussian Blur shader effect | The user can configure the Gaussian blur effect via its function call. |
| F18 | Developer | Add Pixelation shader effect | The user can call a function to add a pixelation effect to their project. Pixelation allows the user to pixelate the screen. |
| F19 | Developer | Configure Pixelation shader effect | The user can configure the pixelation effect via its function call. |
| F20 | Developer | Add HDR Bloom shader effect | The user can call a function to add an HDR bloom effect to their project. HDR bloom allows the user to make surfaces with a certain light amount to glow. |
| F21 | Developer | Configure HDR Bloom shader effect | The user can configure the HDR bloom effect via its function call. |
| F22 | Developer | Add Atmospheric Scattering shader effect | The user can call a function to add an atmospheric scattering effect to their project. Atmospheric Scattering adds an atmosphere sphere that reacts to a light source. This changes the colors of the atmosphere and creates natural effects like sunsets and blue sky during the day. |
| F23 | Developer | Configure Atmospheric Scattering shader effect | The user can configure the atmospheric scattering effect via its function call. |
| F24 | Developer | Add Fresnel Effect shader | The user can call a function to add a Fresnel Effect to an object. |
| F25 | Developer | Configure Fresnel Effect shader | The user can configure the Fresnel Effect via its function call. |

## **4. Success criteria**
Define the metrics and conditions that determine if the beta version is successful.

| **Feature ID** | **Key success criteria** | **Indicator/metric** | **Result** |
|--------------|---------------------------------------|-----------------------|----------------|
| F1 | A user can import OpenGL, Raylib or SFML versions of Shimera. | 3 libraries - All fonctional on Windows and Linux | Achieved (3/3) |
| F2 | A user can use the Color Tint shader effect | Shader is correctly applied to the screen | Achieved |
| F3 | A user can configure the Color Tint shader effect | 1 configurable variable | Achieved (1/1) |
| F4 | A user can use the Distortion shader effect | Shader is correctly applied to the screen | Achieved |
| F5 | A user can configure the Distortion shader effect | 4 configurable variables | Achieved (4/4) |
| F6 | A user can use the Contrast shader effect | Shader is correctly applied to the screen | Achieved |
| F7 | A user can configure the Contrast shader effect | 1 configurable variable | Achieved (1/1) |
| F8 | A user can use the Saturation shader effect | Shader is correctly applied to the screen | Achieved |
| F9 | A user can configure the Saturation shader effect | 1 configurable variable | Achieved (1/1) |
| F10 | A user can use the Brightness shader effect | Shader is correctly applied to the screen | Achieved |
| F11 | A user can configure the Brightness shader effect | 1 configurable variable | Achieved (1/1) |
| F12 | A user can use the Vignette shader effect | Shader is correctly applied to the screen | Achieved |
| F13 | A user can configure the Vignette shader effect | 6 configurable variables | Achieved (6/6) |
| F14 | A user can use the Chromatic Aberration shader effect | Shader is correctly applied to the screen | Achieved |
| F15 | A user can configure the Chromatic Aberration shader effect | 4 configurable variables | Achieved (4/4) |
| F16 | A user can use the Gaussian Blur shader effect | Shader is correctly applied to the screen | Achieved |
| F17 | A user can configure the Gaussian Blur shader effect | 3 configurable variables | Achieved (3/3) |
| F18 | A user can use the Pixelation shader effect | Shader is correctly applied to the screen | Achieved |
| F19 | A user can configure the Pixelation shader effect | 5 configurable variable | Achieved (5/5) |
| F20 | A user can use the HDR Bloom shader effect | Shader is correctly applied to the screen | Achieved |
| F21 | A user can configure the HDR Bloom shader effect | 6 configurable variable | Achieved (6/6) |
| F22 | A user can use the Atmospheric Scattering shader effect | Shader is correctly applied to the screen | Achieved |
| F23 | A user can configure the Atmospheric Scattering shader effect | 15 configurable variables | Achieved (15/15) |
| F24 | A user can use the Fresnel Effect shader | Shader is correctly applied to the object | Achieved |
| F25 | A user can configure the Fresnel Effect shader | 4 configurable variables | Achieved (4/4) |
