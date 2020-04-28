#include "SuperCube.hpp"

SuperCube::SuperCube(const char* grid,                                                                 
                     const unsigned int& width, const unsigned int& height, const unsigned int& depth, 
                     const unsigned int& xSize, const unsigned int& ySize, const unsigned int& zSize){

    std::array<glm::vec3, 8> positions = {
        glm::vec3( 0.f, 0.f, 0.f),
        glm::vec3( 0.f, 1.f, 0.f),
        glm::vec3( 1.f, 0.f, 0.f),
        glm::vec3( 1.f, 1.f, 0.f),
        glm::vec3( 0.f, 0.f, 1.f),
        glm::vec3( 0.f, 1.f, 1.f),
        glm::vec3( 1.f, 0.f, 1.f),
        glm::vec3( 1.f, 1.f, 1.f)
    };
    
    // Index buffer positions
    std::array<unsigned int, 36> indices = {
        0,1,2,
        1,3,2,
        1,5,3,
        5,7,3,
        2,3,6,
        3,7,6,
        0,2,6,
        0,6,4,
        4,5,1,
        4,1,0,
        6,7,4,
        7,5,4
    }; // count = 12 * 3 = 36

    unsigned int counter = 0;

    // Batch a ton of cubes together
    for(unsigned int z = 0; z < zSize; z++){
        for(unsigned int y = 0; y < ySize; y++){
            for(unsigned int x = 0; x < xSize; x++){
                unsigned int idx = z * depth + y * height + x * width;
                std::cout << "Index: " << idx << std::endl;
                if(grid[idx] != '0'){
                    
                    for(auto vert : positions){
                        this->m_Positions.push_back(glm::vec3(x,y,z) + vert);
                    }
                    
                    for(auto ind: indices){
                        this->m_Indices.push_back(ind + counter * 8);
                    }

                    counter++;
                }
            }
        }
    }

/*
    for(auto vert : this->m_Positions){
        std::cout << vert[0] << ", " << vert[1] << ", " << vert[2] << std::endl;
    }
    
    for(auto ind: this->m_Indices){
        std::cout << ind << std::endl;
    }
*/
    std::cout << "Cubes: " << counter << std::endl;

    this->m_PosData = new glm::vec3[this->m_Positions.size()];
    this->m_IndData = new unsigned int[this->m_Indices.size()];

    for(unsigned int i = 0; i < this->m_Positions.size(); i++){
        this->m_PosData[i] = this->m_Positions[i];
    }

    for(unsigned int i = 0; i < this->m_Indices.size(); i++){
        this->m_IndData[i] = this->m_Indices[i];
    }
}

SuperCube::~SuperCube(){
    delete[] this->m_PosData;
    delete[] this->m_IndData;
}
