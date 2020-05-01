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

    protected:
        std::vector<glm::vec3> m_Positions;
        std::vector<unsigned int> m_Indices;
        
        glm::vec3* m_PosData = nullptr;
        unsigned int* m_IndData = nullptr;

};
