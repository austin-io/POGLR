// Precompiled headers for faster compilation
#include "pch.h"

#define DEBUG

// OpenGL Rendering abstraction
#include "src/Renderer.hpp"
#include "src/SuperCube.hpp"

std::string newGrid(const std::string& grid, const SuperCube& sc);

class App : public Renderer {
    public:
        App(const int& h, const int& w, const std::string& title)
        : Renderer(h,w,title) {}
        virtual ~App(){}

        virtual void onCreate() override {

            std::cout << "Generating Grid\n";

            const unsigned int GRID_SIZE = 4;
            for(char i = 0; i < GRID_SIZE; i++){
                for(char j = 0; j < GRID_SIZE; j++){
                    grid += std::bitset<4>(std::rand() % 16).to_string();
                    //grid += std::bitset<16>(std::rand() % 65536).to_string();
                    //grid += std::bitset<24>(0).to_string();
                }
            }

            std::cout << "Finished Generating Grid:\nAttempting to make supercube\n";
            // grid template, height, depth, MaxSize
            sc.create(grid.c_str(), GRID_SIZE, GRID_SIZE*GRID_SIZE, GRID_SIZE);

            std::cout << "SuperCube Generated\n";

            scm.loadData(sc.getVert(), sc.getInd(), sc.getCount(), sc.getICount());

        }

        virtual void onUpdate(double dt) override {
            this->drawMesh(scm);
            //this->mesh.translate(glm::vec3(0.01f));
            //glfwSetWindowShouldClose(win, GLFW_TRUE);
        }

    protected:
        std::string grid = "";
        SuperCube sc = SuperCube();
        Mesh scm = Mesh();

        //Mesh mesh1 = Mesh(), mesh2 = Mesh();
};

int main(int argc, char** argv){
    
    std::srand(std::time(0));

    std::cout << "Main\n";
    
    App app = App(800, 800, "Batching test");

    app.init();
}

std::string newGrid(const std::string& grid, const SuperCube& sc){

    // Rules for Cellular Automata:
    // 1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
    // 2. Any live cell with two or three live neighbours lives on to the next generation.
    // 3. Any live cell with more than three live neighbours dies, as if by overpopulation.
    // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    
    // '0' is dead, '1' is alive
    // There are 6 neighbors in 3D

    std::string out = grid;
    unsigned int idx;

    for(unsigned int z = 0; z < sc.getMaxSize(); z++)
    for(unsigned int y = 0; y < sc.getMaxSize(); y++)
    for(unsigned int x = 0; x < sc.getMaxSize(); x++){
        // index of current cell
        idx = sc.getIndex(x,y,z);
        int neighbors = 0;

        // Get neighbors
        unsigned int nx, ny, nz, nIdx;

        //std::cout << "ID: " << idx << " || Neighbor IDs: \n";

        // Scan through all neighbors
        for(nz = z - 1 >= 0 ? z - 1 : z; nz < z + 2 && nz < sc.getMaxSize(); nz++)
        for(ny = y - 1 >= 0 ? y - 1 : y; ny < y + 2 && ny < sc.getMaxSize(); ny++)
        for(nx = x - 1 >= 0 ? x - 1 : x; nx < x + 2 && nx < sc.getMaxSize(); nx++){
            nIdx = sc.getIndex(nx, ny, nz);
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
