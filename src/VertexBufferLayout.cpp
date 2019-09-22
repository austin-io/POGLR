#include "pch.h"
#include "VertexBufferLayout.hpp"

template<>
void VertexBufferLayout::Push<float>(unsigned int count){
    this->m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
    this->m_Stride += count * VertexBufferElement::getSize(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count){
    this->m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
    this->m_Stride += count * VertexBufferElement::getSize(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count){
    this->m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_FALSE });
    this->m_Stride += count * VertexBufferElement::getSize(GL_UNSIGNED_BYTE);
}
