#pragma once

#include "pch.h"
#include "Renderer.hpp"

struct VertexBufferElement {
    unsigned int count;
    unsigned int type;
    unsigned char normalized;

    static unsigned int getSize(unsigned int type){
        switch(type){
            case GL_FLOAT: return 4; break;
            case GL_UNSIGNED_INT: return 4; break;
            case GL_UNSIGNED_BYTE: return 1; break;
        }
        assert(false);
        return 0;
    }
};

class VertexBufferLayout {
    public:
        VertexBufferLayout()
            : m_Stride(0) {}

        virtual ~VertexBufferLayout(){}

        inline unsigned int getStride() const { return this->m_Stride; }
        inline const std::vector<VertexBufferElement> getElements() const { return this->m_Elements; }

        template<typename T>
        void Push(unsigned int count){
            std::cout << "[ ERROR ] In VertexBufferLayout::Push() - No type specified\n";
            assert(false); 
        }

    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;

};

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count);

template<>
void VertexBufferLayout::Push<unsigned char>(unsigned int count);

template<>
void VertexBufferLayout::Push<float>(unsigned int count);

