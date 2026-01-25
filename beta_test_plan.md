---
title:          Beta Test Plan
subtitle:       Shimera Library
authors:        Léo Maurel, Paul Arbez and Eddy Gardes
module:         G-EIP-700
version:        1.0
---

<!-- # **BETA TEST PLAN - Shimera** -->

## **1. Project context**
Shimera is a library, also referred to as an *SDK* ("*software development kit*"), designed to allow its user (mainly technicians) to implement visual effects on top of their graphical project without using any GPU knowledge.

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
| F3 | Developer | Callable function for shader implementation | Each shader will have a function to be called and modified |
| F4 | Developer | Use colorShift postprocessing shader | Easy-to-implement shader that shifts the screen color. |
| F5 | Developer | Use distortion postprocessing shader | Easy-to-implement shader that distorts the screen. |
| F6 | Developer | Use contrast postprocessing shader | Easy-to-implement shader that changes the screen’s contrast |
| F7 | Developer | Use saturation postprocessing shader | Easy-to-implement shader that saturates the screen. |
| F8 | Developer | Use brightness postprocessing shader | Easy-to-implement shader that brightened the screen. |
| F9 | Developer | Use vignette postprocessing shader | Easy-to-implement shader that adds a vignette to the screen. |
| F10 | Developer | Use chromatic aberration postprocessing shader | Easy-to-implement shader that adds chromatic aberration to the screen. |
| F11 | Developer | Use Gaussian blur postprocessing shader | Easy-to-implement shader that adds Gaussian blur to the screen. |
| F12 | Developer | Use atmospheric scattering shader | Easy-to-implement shader that adds atmospheric scattering to a position in the world. |
| F13 | Developer | Use pixelation postprocessing shader | Easy-to-implement shader that pixelates the screen. |
| F14 | Developer | Use HDR bloom postprocessing shader | Easy-to-implement shader that adds bloom to bright objects. |
| F15 | Developer | Use Fresnel Effect spatial shader | Easy-to-implement spatial shader that implements the Fresnel effect on an object. |

## **4. Success criteria**
Define the metrics and conditions that determine if the beta version is successful.

| **Feature ID** | **Key success criteria** | **Indicator/metric** | **Result** |
|--------------|---------------------------------------|-----------------------|----------------|
| F1 | A user can use the Shimera library on OpenGL |  |  |
| F2 | A user can use the Shimera library on SFML |  |  |
| F3 | A user can implement and modify a shader using a function |  |  |
| F4 | A user can use the distortion postprocessing  shader |  |  |
| F5 | A user can use the color shift postprocessing  shader |  |  |
| F6 | A user can use the contrast postprocessing shader |  |  |
| F7 | A user can use the saturation postprocessing shader |  |  |
| F8 | A user can use brightness postprocessing shader |  |  |
| F9 | A user can use the vignette postprocessing shader |  |  |
| F10 | A user can use the chromatic aberration postprocessing shader |  |  |
| F11 | A user can use the Gaussian blur postprocessing shader |  |  |
| F12 | A user can use the atmospheric scattering shader |  |  |
| F13 | A user can use the pixelation postprocessing shader |  |  |
| F14 | A user can use HDR bloom postprocessing shader |  |  |
| F15 | A user can use Fresnel Effect spatial shader |  |  |
