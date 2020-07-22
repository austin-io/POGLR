#pragma once

#include "pch.h"
#include "Vertex.hpp"

class Mesh {
    public:
        Mesh(){std::cout << __FILE__ << std::endl;}
        Mesh(const std::string& filePath);
        virtual ~Mesh();

        void parseFile(const std::string& filePath);
        void loadData(const Vertex* vertData, const unsigned int* indData, const unsigned int count, const unsigned int iCount);

        inline const Vertex * getVert() const {return this->m_VertData;}
        inline const unsigned int * getInd() const {return this->m_IndData;}

        inline const unsigned int getCount() { return this->m_Vertices.size(); }
        inline const unsigned int getICount() { return this->m_Indices.size(); }

        void translate(const glm::vec3& tran);
        void rotate(const glm::vec3& rot);
        void color(const glm::vec3& col);

        void scale(const float& s);
        void randomize(const float& r);

    protected:
        // Threaded Subprocesses
        void translateTSP(const unsigned int& offset, const unsigned int& range);
        void rotateTSP(const unsigned int& offset, const unsigned int& range);
        void colorTSP(const unsigned int& offset, const unsigned int& range);
        void scaleTSP(const unsigned int& offset, const unsigned int& range);
        void randomizeTSP(const float& r, const unsigned int& offset, const unsigned int& range);

    protected:
        std::vector<Vertex> m_Vertices;
        std::vector<unsigned int> m_Indices;

        glm::vec3 m_Color = glm::vec3(1);
        glm::vec3 m_Pos = glm::vec3(0);

        glm::mat4 m_Rotation = glm::mat4(1.f);

        float m_Scale = 1.f;
        
        Vertex* m_VertData = nullptr;
        unsigned int* m_IndData = nullptr;

};
