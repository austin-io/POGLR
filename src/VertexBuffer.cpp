#include "pch.h"

#include "Renderer.hpp"
#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
    this->create(data, size);
}

VertexBuffer::~VertexBuffer(){
    GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::create(const void* data, unsigned int size){
    // Create a new buffer
    GLCALL(glGenBuffers(1, &m_RendererID));
    // Bind means select the buffer
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); 
    // Populate the buffer with data (type, size, pointer, draw_type)
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

void VertexBuffer::Bind() const {
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); 
}

void VertexBuffer::Unbind() const {
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
}

void VertexBuffer::setData(const void* data, unsigned int size){
    this->Bind();
    // Populate the buffer with data (type, offset, size, pointer)
    GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}