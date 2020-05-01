#include "pch.h"

#pragma once

enum ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

struct ShaderSources {
    std::string vertexShader;
    std::string fragShader;
};

class Shader {
    public:
        Shader(){std::cout << __FILE__ << std::endl;}
        Shader(const std::string& filepath);
        void create(const std::string& filepath);
        virtual ~Shader();

        void Bind() const;
        void Unbind() const;

        // Set Uniforms
        void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
        void setUniform4f(const std::string& name, float, float, float, float);
        void setUniform3f(const std::string& name, const glm::vec3& u_Vec);
    protected:
        // Private Variables
        std::string m_Filepath;
        unsigned int m_RendererID;
        
        // Caching for uniforms
        std::map<std::string, int> m_UniformLocations;

        // Private methods
        ShaderSources parseShader(const std::string& filepath);
        int getUniformLocations(const std::string& name);
        unsigned int compileShader(const std::string& source, unsigned int type);
        unsigned int createShader(const std::string& vertexShader, const std::string& fragShader);

};
