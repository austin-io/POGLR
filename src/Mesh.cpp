#include "Mesh.hpp"

Mesh::Mesh(const std::string& filePath){
    this->parseFile(filePath);
}

Mesh::~Mesh(){}

void Mesh::parseFile(const std::string& filePath){
    std::ifstream inFile(filePath);

    if(!inFile){
        std::cout << "File failed to load" << std::endl;
    } else {
        std::string line;
        std::stringstream ss;
        char tmp;
        glm::vec3 data;
        unsigned int index[3];
        while(getline(inFile, line)){
            ss = std::stringstream(line);
            if(line.find("v ") != std::string::npos){
                ss >> tmp >> data[0] >> data[1] >> data[2];
                this->m_Positions.push_back(data);
            } else if(line.find("f ") != std::string::npos) {
                ss >> tmp >> index[0] >> index[1] >> index[2];
                this->m_Indices.push_back(index[0] - 1);
                this->m_Indices.push_back(index[1] - 1);
                this->m_Indices.push_back(index[2] - 1);
            }
        }
        std::cout << filePath << " Vertex Count: " << m_Positions.size() << std::endl;
        std::cout << filePath << " Index Count: " << m_Indices.size() / 3 << std::endl;
    }

    inFile.close();
}
