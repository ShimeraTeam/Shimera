// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without using any GPU knowledge
// Copyright (C) 2025-2026 The Shimera Authors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#version 330 core

in vec2 texCoords;

uniform sampler2D u_screenTexture;
uniform sampler2D u_depthTexture;
uniform vec3 u_cameraPos;
uniform vec3 u_lightDir;
uniform float u_cameraNear;
uniform float u_cameraFar;
uniform mat4 u_cameraInverseProjection;
uniform mat4 u_cameraWorldMatrix;
uniform vec3 u_planetCenter;
uniform float u_planetRadius;
uniform vec3 u_scatterCoefficients;
uniform float u_atmosphereRadius;
uniform float u_densityFalloff;
uniform int u_opticalDepthSamples; // should not be <1!
uniform int u_inScatteringPoints; // should not be <1!


float linearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // convert to ndc
    return (2.0 * u_cameraNear * u_cameraFar) / (u_cameraFar + u_cameraNear - z * (u_cameraFar - u_cameraNear));
}

vec2 raySphereIntersect(vec3 sphereCenter, float sphereRadius, vec3 rayOrigin, vec3 rayDirection)
{
    vec3 offset = rayOrigin - sphereCenter;
    float a = dot(rayDirection, rayDirection);
    float b = 2.0 * dot(offset, rayDirection);
    float c = dot(offset, offset) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0) {
        return vec2(-1.0, 0.0);
    }

    float sqrtDiscriminant = sqrt(discriminant);
    float t0 = (-b - sqrtDiscriminant) / (2.0 * a);
    float t1 = (-b + sqrtDiscriminant) / (2.0 * a);

    if (t1 < 0.0) {
        return vec2(-1.0, 0.0);
    }

    if (t0 < 0.0) {
        return vec2(0.0, t1);
    }

    return vec2(t0, t1 - t0);
}

float densityAtPoint(vec3 point) {
    float heightAboveSurface = length(point - u_planetCenter) - u_planetRadius;
    float scaledHeight = heightAboveSurface / (u_atmosphereRadius - u_planetRadius);
    float localDensity = exp(-scaledHeight * u_densityFalloff) * (1.0 - scaledHeight);
    return localDensity;
}

float opticalDepth(vec3 rayOrigin, vec3 rayDirection, float rayLength) {
    vec3 samplePoint = rayOrigin;
    float stepSize = rayLength / float(u_opticalDepthSamples - 1);
    float opticalDepth = 0.0;

    for (int i = 0; i < u_opticalDepthSamples; i++) {
        float localDensity = densityAtPoint(samplePoint);
        opticalDepth += localDensity * stepSize;
        samplePoint += rayDirection * stepSize;
    }
    return opticalDepth;
}

vec3 calculateLightInteraction(vec3 rayOrigin, vec3 rayDirection, float rayLength, vec3 finalColor) {
    vec3 inScatterPoint = rayOrigin;
    float stepSize = rayLength / float(u_inScatteringPoints - 1);
    vec3 inScatteredLight = vec3(0.0);
    float viewRayOpticalDepth = 0.0;

    for (int i = 0; i < u_inScatteringPoints; i++) {
        float sunRayLength = raySphereIntersect(u_planetCenter, u_atmosphereRadius, inScatterPoint, u_lightDir).y;
        float sunRayOpticalDepth = opticalDepth(inScatterPoint, u_lightDir, sunRayLength);
        viewRayOpticalDepth = opticalDepth(inScatterPoint, -rayDirection, stepSize * float(i));
        vec3 transmitance = exp(-(sunRayOpticalDepth + viewRayOpticalDepth) * u_scatterCoefficients);
        float localDensity = densityAtPoint(inScatterPoint);

        inScatteredLight += localDensity * transmitance * u_scatterCoefficients * stepSize;
        inScatterPoint += rayDirection * stepSize;
    }
    float originalColorTransmitance = exp(-viewRayOpticalDepth);
    return finalColor * originalColorTransmitance + inScatteredLight;
}

void main()
{
    vec4 originalColor = texture2D(u_screenTexture, texCoords);

    // view ray from screen uv
    vec2 ndc = texCoords * 2.0 - 1.0; // convert uv to clip space
    vec4 clipPos = vec4(ndc, -1.0, 1.0); // create clip space position (near plane)
    vec4 viewPos = u_cameraInverseProjection * clipPos; // transform to view space
    viewPos /= viewPos.w;
    vec3 viewVector = (u_cameraWorldMatrix * vec4(viewPos.xyz, 0.0)).xyz; // to world space

    vec3 rayOrigin = u_cameraPos;
    vec3 rayDirection = normalize(viewVector);
    float screenDepthNonLinear = texture2D(u_depthTexture, texCoords).r;
    bool isBackground = screenDepthNonLinear >= 0.9999;

    float rayDistance = 0.0;
    if (!isBackground) {
        float linearDepth = linearizeDepth(screenDepthNonLinear);

        // reconstruct view space position for depth
        vec4 clipPosDepth = vec4(ndc, screenDepthNonLinear * 2.0 - 1.0, 1.0);
        vec4 viewPosDepth = u_cameraInverseProjection * clipPosDepth;
        viewPosDepth /= viewPosDepth.w;

        vec3 worldPos = (u_cameraWorldMatrix * vec4(viewPosDepth.xyz, 1.0)).xyz;
        rayDistance = length(worldPos - rayOrigin);
    }

    // atmosphere intersection
    vec2 atmosphereHit = raySphereIntersect(u_planetCenter, u_atmosphereRadius, rayOrigin, rayDirection);
    float distanceToAtmosphere = atmosphereHit.x;
    float distanceThroughAtmosphere = atmosphereHit.y;

    vec3 finalColor = originalColor.rgb;

    if (distanceToAtmosphere >= 0.0 && distanceThroughAtmosphere > 0.0) {
        float dstThroughAtmosphere = distanceThroughAtmosphere;

        if (!isBackground) {
            if (rayDistance < distanceToAtmosphere) {
                dstThroughAtmosphere = 0.0;
            } else if (rayDistance < distanceToAtmosphere + distanceThroughAtmosphere) {
                dstThroughAtmosphere = rayDistance - distanceToAtmosphere;
            }
        }

        if (dstThroughAtmosphere > 0.0) {
            vec3 pointInAtmosphere = rayOrigin + rayDirection * distanceToAtmosphere;
            vec3 light = calculateLightInteraction(pointInAtmosphere, rayDirection, dstThroughAtmosphere, finalColor);
            finalColor = light;
        }
    }

    gl_FragColor = vec4(finalColor, 1.0);
}
