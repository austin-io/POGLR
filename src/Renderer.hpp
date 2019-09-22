#pragma once

#include "pch.h"

#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

// Creates a line break if an error is found
#define ASSERT(x) if(!(x)) assert(x);
#define DEBUG

// GLCALL wraps a function between our error handling functions
#if defined(DEBUG)
    #define GLCALL(x) GLClearErrors(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCALL(x) x 
#endif

void GLClearErrors();
bool GLLogCall(const char*, const char*, int);

class Renderer {
    public:
        Renderer();
        virtual ~Renderer();

        void clear() const;
        void drawTris(const VertexArray& va, const IndexBuffer& ibo, const Shader& shader);
    protected:

};