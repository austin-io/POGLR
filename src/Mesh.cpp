#include "Mesh.hpp"

Mesh::Mesh(const std::string& filePath){
    this->parseFile(filePath);
}

Mesh::~Mesh(){}

void Mesh::parseFile(const std::string& filePath){
    std::vector<glm::vec3> posV;
    std::vector<unsigned int> indV;
    
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
                posV.push_back(data);
            } else if(line.find("f ") != std::string::npos) {
                ss >> tmp >> index[0] >> index[1] >> index[2];
                indV.push_back(index[0] - 1);
                indV.push_back(index[1] - 1);
                indV.push_back(index[2] - 1);
            }
        }
        std::cout << filePath << " Vertex Count: " << posV.size() << std::endl;
        std::cout << filePath << " Index Count: " << indV.size() / 3 << std::endl;
    }

    inFile.close();

    this->m_Positions.reserve(posV.size());
    this->m_Indices.reserve(indV.size());
    
    std::copy(posV.begin(), posV.end(), this->m_Positions.begin());  
    std::copy(indV.begin(), indV.end(), this->m_Indices.begin()); 
}
