#pragma once

#include "pch.h"

class SuperCube {
    public:
        SuperCube(const char* grid, const unsigned int& height, const unsigned int& depth, const unsigned int& MAX_SIZE);

        ~SuperCube();

        inline const glm::vec3* getVert() const {return this->m_PosData;}
        inline const unsigned int* getInd() const {return this->m_IndData;}

        inline const unsigned int getCount() const {return this->m_Positions.size();}
        inline const unsigned int getICount() const {return this->m_Indices.size();}
        
        inline const unsigned int getHeight()  const {return this->m_Height;}
        inline const unsigned int getDepth()   const {return this->m_Depth;}
        inline const unsigned int getMaxSize() const {return this->m_MaxSize;}

        inline const unsigned int getIndex(const unsigned int& x, const unsigned int& y, const unsigned int& z) const {return z*this->m_Depth + y*this->m_Height + x;}

        void updateGrid(const char* grid);

    private:
        std::vector<glm::vec3> m_Positions;
        std::vector<unsigned int> m_Indices;

        glm::vec3* m_PosData;
        unsigned int* m_IndData;

        unsigned int m_Height, m_Depth, m_MaxSize;
};
