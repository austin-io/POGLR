#include "Renderer.hpp"

void GLClearErrors(){
    // Remove all previous error messages if any
    while(GLenum error = glGetError());
}

bool GLLogCall(const char* functionName, const char* filepath, int lineNumber){
    // Output error messages if any and their location
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] - (" << error << ") '" 
                  << functionName 
                  << "' In file: '" << filepath 
                  << "' line:" << lineNumber << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer(){}
Renderer::~Renderer(){}

void Renderer::clear() const {
    // Clear screen to grey
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1));
}

void Renderer::drawTris(const VertexArray& va, const IndexBuffer& ibo, const Shader& shader){
    va.Bind();
    ibo.Bind();
    shader.Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}