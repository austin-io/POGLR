#pragma once

#include "pch.h"

class SuperCube {
    public:
        SuperCube(
                const char* grid, 
                const unsigned int& width, const unsigned int& height, const unsigned int& depth, 
                const unsigned int& xSize, const unsigned int& ySize, const unsigned int& zSize);
        ~SuperCube();

        inline const glm::vec3* getVert() const {return this->m_PosData;}
        inline const unsigned int* getInd() const {return this->m_IndData;}

        inline const unsigned int getCount() const {return this->m_Positions.size();}
        inline const unsigned int getICount() const {return this->m_Indices.size();}

    private:
        std::vector<glm::vec3> m_Positions;
        std::vector<unsigned int> m_Indices;

        glm::vec3* m_PosData;
        unsigned int* m_IndData;
};
