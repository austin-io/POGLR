#include "Shader.hpp"

#include "Renderer.hpp"

Shader::Shader(const std::string& filepath)
    : m_Filepath(filepath), m_RendererID(0) {
    
    ShaderSources sources = this->parseShader(filepath);
    this->m_RendererID = this->createShader(sources.vertexShader, sources.fragShader);
}

Shader::~Shader(){
    GLCALL(glDeleteProgram(this->m_RendererID));
}

void Shader::Bind() const {
    GLCALL(glUseProgram(this->m_RendererID));
}

void Shader::Unbind() const {
    GLCALL(glUseProgram(0));
}

int Shader::getUniformLocations(const std::string& name){
    
    if(this->m_UniformLocations.find(name) != this->m_UniformLocations.end())
        return this->m_UniformLocations[name];

    GLCALL(int location = glGetUniformLocation(this->m_RendererID, name.c_str()));
    if(location == -1)
        std::cout << "Uniform " << name << " doesn't exist\n";
    else
        this->m_UniformLocations[name] = location;

    return this->m_UniformLocations[name];
}

void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3){
    GLCALL(glUniform4f(this->getUniformLocations(name), f0, f1, f2, f3));
}

void Shader::setUniform3f(const std::string& name, const glm::vec3& u_Vec){
    GLCALL(glUniform3f(this->getUniformLocations(name), u_Vec[0], u_Vec[1], u_Vec[2]));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix){
    GLCALL(glUniformMatrix4fv(this->getUniformLocations(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type){
    
    GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    
    // Select the shader address and compile
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));
    
    // Error handling
    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    
    // Something went wrong
    if(result == GL_FALSE){
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        
        // message must be a const length
        // alloca() dynamically allocates this memory on the stack
        char* message = (char*)alloca(length * sizeof(char));
        GLCALL(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile shader!\n" << message << std::endl;
        
        // Delete shader because it failed
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

ShaderSources Shader::parseShader(const std::string& filepath){
    
    std::ifstream inFile(filepath);
    if(!inFile) std::cout << "Failed to open '" << filepath << "'\n"; 

    std::string line;
    std::stringstream ss[2];
    
    ShaderType type = ShaderType::NONE;

    while(getline(inFile, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            } else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << std::endl;
        }
    }

    inFile.close();

    return { ss[ShaderType::VERTEX].str(), ss[ShaderType::FRAGMENT].str() };
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragShader){
    
    // id of the current program
    GLCALL(unsigned int program = glCreateProgram());
    
    // vs and fs hold the id to their shaders
    unsigned int vs = this->compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = this->compileShader(fragShader, GL_FRAGMENT_SHADER);

    // Give the shaders to our program
    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    
    // Make sure our program doesn't have any problems
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    // Remove shaders after saving the program
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;
}
