#pragma once

#include "pch.h"

class Mesh {
    public:
        Mesh(){std::cout << __FILE__ << std::endl;}
        Mesh(const std::string& filePath);
        virtual ~Mesh();

        void parseFile(const std::string& filePath);
        void loadData(const glm::vec3* posData, const unsigned int* indData, const unsigned int count, const unsigned int iCount);

        inline const glm::vec3 * getVert() const {return this->m_PosData;}
        inline const unsigned int * getInd() const {return this->m_IndData;}

        inline const unsigned int getCount() { return this->m_Positions.size(); }
        inline const unsigned int getICount() { return this->m_Indices.size(); }

        void translate(const glm::vec3& tran);
        void rotate(const glm::vec3& rot);
        void color(const glm::vec3& col);

        void scale(const float& s);
        void randomize(const float& r);

    protected:
        std::vector<glm::vec3> m_Positions;
        std::vector<unsigned int> m_Indices;

        glm::vec3 m_Color = glm::vec3(1);
        glm::vec3 m_Pos = glm::vec3(0);
        glm::mat4 m_Rotation = glm::mat4(1.f);

        float m_Scale = 1.f;
        
        glm::vec3* m_PosData = nullptr;
        unsigned int* m_IndData = nullptr;

};
