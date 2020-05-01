#include "pch.h"

#include "Renderer.hpp"
#include "VertexArray.hpp"
//#include "VertexBuffer.hpp"
//#include "VertexBufferLayout.hpp"

VertexArray::VertexArray(){
    std::cout << __FILE__ << std::endl;
}

VertexArray::~VertexArray(){
    GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::create(){
    GLCALL(glGenVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const {
    GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
    GLCALL(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
    Bind();
    vb.Bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;
    for(unsigned int i = 0; i < elements.size(); i++){
        const auto& element = elements[i];
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::getSize(element.type);
    }
}
