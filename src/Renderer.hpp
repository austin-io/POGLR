#pragma once

#include "pch.h"

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

#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"
#include "Mesh.hpp"

class Renderer {
    public:
        Renderer(const int& h, const int& w, const std::string& title);
        virtual ~Renderer();

        void init();
        void coreUpdate();
        void drawMesh(Mesh& mesh);
        void flush();
        void clear() const;

        inline static const double fRand() {return (static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX));}

        virtual void onCreate();
        virtual void onUpdate(double);
        //virtual void onEvent(GLFWwindow* win, int key, int scancode, int action, int mods);

    protected:
        // Main window provided by GLFW (not created yet)
        GLFWwindow* win;

        const unsigned long MAXSIZE = 1000000;
        unsigned long m_Count, m_ICount;

        glm::vec3* m_VertData;
        unsigned int* m_IndData;

        double dTime;

        //Mesh mesh = Mesh(); 
        VertexBuffer vb = VertexBuffer();
        VertexBufferLayout vbl = VertexBufferLayout();
        VertexArray va = VertexArray();
        IndexBuffer ib = IndexBuffer(); 
        Shader shader = Shader();

};
