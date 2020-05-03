// Precompiled headers for faster compilation
#include "pch.h"

#define DEBUG

// OpenGL Rendering abstraction
#include "src/Renderer.hpp"
#include "src/SuperCube.hpp"

struct CubeGen{
    unsigned int 
        Height, 
        Depth, 
        MaxSize;
    inline const unsigned int getIndex(const unsigned int& x, const unsigned int& y, const unsigned int& z) const {return z*this->Depth + y*this->Height + x;}
};

std::string newGrid(const std::string& grid, const CubeGen& cg);

class App : public Renderer {
    public:
        App(const int& h, const int& w, const std::string& title)
        : Renderer(h,w,title) {}
        virtual ~App(){}

        virtual void onCreate() override {

            std::cout << "Generating Grid\n";

            for(char i = 0; i < CHUNK_SIZE; i++){
                for(char j = 0; j < CHUNK_SIZE; j++){
                    grid += std::bitset<16>(std::rand() % 65500).to_string();
                    grid += std::bitset<16>(std::rand() % 65500).to_string();
                    grid += std::bitset<16>(std::rand() % 65500).to_string();
                    grid += std::bitset<16>(std::rand() % 65500).to_string();
                    
                    grid += std::bitset<16>(std::rand() % 65500).to_string();
                    grid += std::bitset<16>(std::rand() % 65500).to_string();
                    grid += std::bitset<16>(std::rand() % 65500).to_string();
                    grid += std::bitset<16>(std::rand() % 65500).to_string();
                }
            }

            this->cube.parseFile("./models/teapot.obj");

            this->cube.scale(this->scale);

        }

        virtual void onUpdate(double dt) override {
            
            //std::cout << "Delta time: " << dt << "s | " << dt * 1000 << "ms" << std::endl;
            
            //* Spinny cubes
            // Draw Red cube
            //this->cube.scale(1 + App::fRand() * 0.5);
            //this->cube.scale(1);
            this->cube.rotate(glm::vec3(0));
            this->cube.color(glm::vec3(1,0.2,0.2));
            this->cube.translate(glm::vec3(1,0,0));
            this->drawMesh(this->cube);

            // Draw Blue cube
            //this->cube.scale(1);
            this->cube.rotate(glm::vec3(90*std::sin(5 * counter), 0, 0));
            this->cube.color(glm::vec3(0,0.5,1));
            this->cube.translate(glm::vec3(-2, 0,0));
            //this->cube.translate(glm::vec3(-1, std::sin(5 * counter), std::cos(5 * counter)));
            this->drawMesh(this->cube);
            //*/

            counter += dt;

            /*// Cellular Automata
            drawCubes();
            //*/

            //glfwSetWindowShouldClose(this->win, GLFW_TRUE);
        }

        void drawCubes(){
            for(unsigned int z = 0; z < cg.MaxSize; z++){
                for(unsigned int y = 0; y < cg.MaxSize; y++){
                    for(unsigned int x = 0; x < cg.MaxSize; x++){
                        unsigned int idx = cg.getIndex(x,y,z);

                        if(this->grid[idx] == '1'){
                            this->cube.translate(glm::vec3(x,y,z) * this->scale);
                            this->drawMesh(this->cube);
                        }
                    }
                }
            }
            this->grid = newGrid(this->grid, this->cg);
        }

    protected:
        const unsigned int CHUNK_SIZE = 128;
        const float scale = 0.5f;

        double counter = 0;

        std::string grid = "";
        CubeGen cg = {CHUNK_SIZE, CHUNK_SIZE*CHUNK_SIZE, CHUNK_SIZE};
        Mesh cube = Mesh(), m1 = Mesh();
};

int main(int argc, char** argv){
    
    std::srand(std::time(0));

    std::cout << "Main\n";
    
    App app = App(800, 800, "Batching test");

    app.init();
}

std::string newGrid(const std::string& grid, const CubeGen& cg){

    // Rules for Cellular Automata:
    // 1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
    // 2. Any live cell with two or three live neighbours lives on to the next generation.
    // 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
    // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    
    // '0' is dead, '1' is alive
    // There are 6 neighbors in 3D

    std::string out = grid;
    unsigned int idx;

    for(unsigned int z = 0; z < cg.MaxSize; z++)
    for(unsigned int y = 0; y < cg.MaxSize; y++)
    for(unsigned int x = 0; x < cg.MaxSize; x++){
        // index of current cell
        idx = cg.getIndex(x,y,z);
        int neighbors = 0;

        // Get neighbors
        unsigned int nx, ny, nz, nIdx;

        //std::cout << "ID: " << idx << " || Neighbor IDs: \n";

        // Scan through all neighbors
        for(nz = z - 1 >= 0 ? z - 1 : z; nz < z + 2 && nz < cg.MaxSize; nz++)
        for(ny = y - 1 >= 0 ? y - 1 : y; ny < y + 2 && ny < cg.MaxSize; ny++)
        for(nx = x - 1 >= 0 ? x - 1 : x; nx < x + 2 && nx < cg.MaxSize; nx++){
            nIdx = cg.getIndex(nx, ny, nz);
            //std::cout << nIdx << ", ";
            if(nIdx != idx && grid[nIdx] == '1') neighbors++;
        }

        //std::cout << "\nTotal: " << neighbors << std::endl;
        
        // Alive
        if(grid[idx] != '0'){
            
            if(neighbors < 3 || neighbors > 5){
                out[idx] = '0';
            } else {
                out[idx] = '1';
            }

        } 
        // Dead
        else {
            
            if(neighbors == 3){
                out[idx] = '1';
            } else {
                out[idx] = '0';
            }
        }

    }
    
    //std::cout << "New Grid: " << out << std::endl;

    return out;
}
