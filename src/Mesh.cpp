#include "Mesh.hpp"

Mesh::Mesh(const std::string& filePath){
    this->parseFile(filePath);
}

Mesh::~Mesh(){
    delete[] this->m_PosData;
    delete[] this->m_IndData;
}

void Mesh::parseFile(const std::string& filePath){
    //std::vector<glm::vec3> posV;
    //std::vector<unsigned int> indV;
    
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
        std::cout << filePath << " Vertex Count: " << this->m_Positions.size() << std::endl;
        std::cout << filePath << " Index Count: " << this->m_Indices.size() << std::endl;
    }

    inFile.close();

    this->m_PosData = new glm::vec3[this->m_Positions.size()];
    this->m_IndData = new unsigned int[this->m_Indices.size()];

    for(unsigned int i = 0; i < this->m_Positions.size(); i++){
        this->m_PosData[i] = this->m_Positions[i];
    }

    for(unsigned int i = 0; i < this->m_Indices.size(); i++){
        this->m_IndData[i] = this->m_Indices[i];
    }
}

void Mesh::loadData(const glm::vec3* posData, const unsigned int* indData, const unsigned int count, const unsigned int iCount){
    this->m_Positions.clear();
    this->m_Indices.clear();

    if(this->m_PosData != nullptr || this->m_IndData != nullptr){
        delete[] this->m_PosData;
        delete[] this->m_IndData;
    }

    this->m_PosData = new glm::vec3[count];
    this->m_IndData = new unsigned int[iCount];

    for(unsigned int i = 0; i < count; i++){
        this->m_PosData[i] = posData[i];
        this->m_Positions.push_back(posData[i]);
    }

    for(unsigned int i = 0; i < iCount; i++){
        this->m_IndData[i] = indData[i];
        this->m_Indices.push_back(indData[i]);
    }
}

void Mesh::translate(const glm::vec3& tran){
    for(unsigned int i = 0; i < this->getCount(); i++){
        this->m_PosData[i] = this->m_Positions[i] * this->m_Scale + tran;
    }
}

void Mesh::rotate(const glm::vec3& rot){
    //for(unsigned int i = 0; i < this->getCount(); i++){        
    //    this->m_PosData[i] = glm::rotate(this->m_PosData[i], glm::radians(rot[]), glm::vec3())
    //    this->m_PosData[i] = glm::rotate(this->m_PosData[i], glm::radians(rot[]), glm::vec3())
    //    this->m_PosData[i] = glm::rotate(this->m_PosData[i], glm::radians(rot[]), glm::vec3())
    //}
}

void Mesh::scale(const float& s){
    this->m_Scale = s;
}

void Mesh::randomize(const float& r){
    for(unsigned int i = 0; i < this->getCount(); i++){
        this->m_PosData[i] = (this->m_Positions[i] * this->m_Scale) 
            + glm::vec3(
                ((static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX)) - 0.5) * r,
                ((static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX)) - 0.5) * r,
                ((static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX)) - 0.5) * r
                );
    }
}