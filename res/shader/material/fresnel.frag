#version 330 core
in vec3 vWorldPos;
in vec3 vNormal;
out vec4 color;

uniform vec3  u_cameraPos;
uniform vec3  u_color;
uniform float u_power;
uniform float u_reflectance;
uniform float u_intensity;

void main() {
    vec3 N = normalize(vNormal);
    vec3 V = normalize(u_cameraPos - vWorldPos);
    float fresnel = u_reflectance + (1.0 - u_reflectance) * pow(1.0 - max(dot(N, V), 0.0), u_power);
    fresnel *= u_intensity;
    color = vec4(u_color * fresnel, fresnel);
}