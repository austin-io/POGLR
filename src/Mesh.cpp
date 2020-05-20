#include "Mesh.hpp"
#include "Renderer.hpp"

Mesh::Mesh(const std::string& filePath){
    this->parseFile(filePath);
}

Mesh::~Mesh(){
    delete[] this->m_VertData;
    delete[] this->m_IndData;
}

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
                this->m_Vertices.push_back({data, this->m_Color});
            } else if(line.find("f ") != std::string::npos) {
                ss >> tmp >> index[0] >> index[1] >> index[2];
                this->m_Indices.push_back(index[0] - 1);
                this->m_Indices.push_back(index[1] - 1);
                this->m_Indices.push_back(index[2] - 1);
            }
        }
        std::cout << filePath << " Vertex Count: " << this->m_Vertices.size() << std::endl;
        std::cout << filePath << " Index Count: " << this->m_Indices.size() << std::endl;
    }

    inFile.close();

    this->m_VertData = new Vertex[this->m_Vertices.size()];
    this->m_IndData = new unsigned int[this->m_Indices.size()];

    for(unsigned int i = 0; i < this->m_Vertices.size(); i++){
        this->m_VertData[i] = this->m_Vertices[i];
    }

    for(unsigned int i = 0; i < this->m_Indices.size(); i++){
        this->m_IndData[i] = this->m_Indices[i];
    }
}

void Mesh::loadData(const Vertex* vertData, const unsigned int* indData, const unsigned int count, const unsigned int iCount){
    this->m_Vertices.clear();
    this->m_Indices.clear();

    if(this->m_VertData != nullptr || this->m_IndData != nullptr){
        delete[] this->m_VertData;
        delete[] this->m_IndData;
    }

    this->m_VertData = new Vertex[count];
    this->m_IndData = new unsigned int[iCount];

    for(unsigned int i = 0; i < count; i++){
        this->m_VertData[i] = vertData[i];
        this->m_Vertices.push_back(vertData[i]);
    }

    for(unsigned int i = 0; i < iCount; i++){
        this->m_IndData[i] = indData[i];
        this->m_Indices.push_back(indData[i]);
    }
}

void Mesh::translate(const glm::vec3& tran){
    this->m_Pos = tran;
    
    std::thread t1(&Mesh::translateTSP, this, tran, 0, this->getCount()/4);
    std::thread t2(&Mesh::translateTSP, this, tran, this->getCount()/4, this->getCount()/2);
    std::thread t3(&Mesh::translateTSP, this, tran, this->getCount()/2, 3 * this->getCount()/4);
    std::thread t4(&Mesh::translateTSP, this, tran, 3 * this->getCount()/4, this->getCount());

    t1.join();
    t2.join();
    t3.join();
    t4.join();

}

void Mesh::rotate(const glm::vec3& rot){
    
    this->m_Rotation = glm::mat4(1);
    this->m_Rotation = glm::rotate(this->m_Rotation, glm::radians(rot[0]), glm::vec3(1.f, 0.f, 0.f));
    this->m_Rotation = glm::rotate(this->m_Rotation, glm::radians(rot[1]), glm::vec3(0.f, 1.f, 0.f));
    this->m_Rotation = glm::rotate(this->m_Rotation, glm::radians(rot[2]), glm::vec3(0.f, 0.f, 1.f));

    std::thread t1(&Mesh::rotateTSP, this, 0, this->getCount()/4);
    std::thread t2(&Mesh::rotateTSP, this, this->getCount()/4, this->getCount()/2);
    std::thread t3(&Mesh::rotateTSP, this, this->getCount()/2, 3 * this->getCount()/4);
    std::thread t4(&Mesh::rotateTSP, this, 3 * this->getCount()/4, this->getCount());

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void Mesh::color(const glm::vec3& col){
    this->m_Color = col;

    std::thread t1(&Mesh::colorTSP, this, 0, this->getCount()/4);
    std::thread t2(&Mesh::colorTSP, this, this->getCount()/4, this->getCount()/2);
    std::thread t3(&Mesh::colorTSP, this, this->getCount()/2, 3 * this->getCount()/4);
    std::thread t4(&Mesh::colorTSP, this, 3 * this->getCount()/4, this->getCount());

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void Mesh::scale(const float& s){
    this->m_Scale = s;
    
    std::thread t1(&Mesh::scaleTSP, this, 0, this->getCount()/4);
    std::thread t2(&Mesh::scaleTSP, this, this->getCount()/4, this->getCount()/2);
    std::thread t3(&Mesh::scaleTSP, this, this->getCount()/2, 3 * this->getCount()/4);
    std::thread t4(&Mesh::scaleTSP, this, 3 * this->getCount()/4, this->getCount());

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void Mesh::randomize(const float& r){
    std::thread t1(&Mesh::randomizeTSP, this, r, 0, this->getCount()/4);
    std::thread t2(&Mesh::randomizeTSP, this, r, this->getCount()/4, this->getCount()/2);
    std::thread t3(&Mesh::randomizeTSP, this, r, this->getCount()/2, 3 * this->getCount()/4);
    std::thread t4(&Mesh::randomizeTSP, this, r, 3 * this->getCount()/4, this->getCount());

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void Mesh::translateTSP(const glm::vec3& tran, const unsigned int& beginInd, const unsigned int& endInd){
    for(unsigned int i = beginInd; i < endInd; i++){
        this->m_VertData[i].position = glm::vec3(glm::vec4(this->m_Vertices[i].position * this->m_Scale, 1) * this->m_Rotation)  + this->m_Pos;
    }
}

void Mesh::rotateTSP(const unsigned int& beginInd, const unsigned int& endInd){
    for(unsigned int i = beginInd; i < endInd; i++){
        this->m_VertData[i].position = glm::vec3(glm::vec4(this->m_Vertices[i].position * this->m_Scale, 1) * this->m_Rotation)  + this->m_Pos;
    }
}

void Mesh::colorTSP(const unsigned int& beginInd, const unsigned int& endInd){
    for(unsigned int i = beginInd; i < endInd; i++){
        this->m_VertData[i].color = this->m_Color;
    }
}

void Mesh::scaleTSP(const unsigned int& beginInd, const unsigned int& endInd){
    for(unsigned int i = beginInd; i < endInd; i++){
        this->m_VertData[i].position = glm::vec3(glm::vec4(this->m_Vertices[i].position * this->m_Scale, 1) * this->m_Rotation)  + this->m_Pos;
    }
}

void Mesh::randomizeTSP(const float& r, const unsigned int& beginInd, const unsigned int& endInd){
    for(unsigned int i = beginInd; i < endInd; i++){
        this->m_VertData[i].position = glm::vec3(glm::vec4(this->m_Vertices[i].position * this->m_Scale, 1) * this->m_Rotation)  + this->m_Pos
            + glm::vec3(
                (Renderer::fRand() - 0.5) * r,
                (Renderer::fRand() - 0.5) * r,
                (Renderer::fRand() - 0.5) * r
                );
    }
}
