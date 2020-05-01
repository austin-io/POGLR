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

        virtual void onCreate();
        virtual void onUpdate(double);

    protected:
        // Main window provided by GLFW (not created yet)
        GLFWwindow* win;

        const unsigned long MAXSIZE = 10000;
        unsigned long m_Count, m_ICount;

        std::vector<glm::vec3> m_Vertices;
        std::vector<unsigned int> m_Indices;

        glm::vec3* m_VertData;
        unsigned int* m_IndData;

        double dTime;

        //Mesh mesh = Mesh(); //("./models/cube.obj");
        VertexBuffer vb = VertexBuffer(); //(this->m_VertData, this->MAXSIZE * 3 * sizeof(float));
        VertexBufferLayout vbl = VertexBufferLayout();
        VertexArray va = VertexArray();
        IndexBuffer ib = IndexBuffer(); //(this->m_IndData, this->MAXSIZE * 3);
        Shader shader = Shader();//("res/shaders/base.shader");
        
};
