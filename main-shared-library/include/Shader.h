#pragma once

#include <string>
#include <unordered_map>

class Shader {
    public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void setUniform1i(const std::string& name, int value);
    void setUniform1f(const std::string& name, float value);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

    private:
        unsigned int m_RendererID;
        int GetUniformLocation(const std::string& name);
        unsigned int CreateShader(const std::string& vertexPath, const std::string& fragmentPath);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        std::string LoadShaderFromFile(const std::string& filepath);
        std::string m_VertexPath;
        std::string m_FragmentPath;
        std::unordered_map<std::string, int> m_UniformLocationCache;
};