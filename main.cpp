/*
 * Learning OpenGL 
 * Experimental program following TheCherno tutorial series
 * */

// Precompiled headers for faster compilation
#include "pch.h"

#define DEBUG

// OpenGL Rendering abstraction
#include "src/Renderer.hpp"
#include "src/VertexBuffer.hpp"
#include "src/VertexBufferLayout.hpp"
#include "src/IndexBuffer.hpp"
#include "src/Shader.hpp"
#include "src/VertexArray.hpp"
#include "src/Mesh.hpp"
#include "src/SuperCube.hpp"

/*
struct ShaderSources{
    std::string vertexShader;
    std::string fragShader;
};

enum ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};
*/

// Function called when event happens
void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods);

//static ShaderSources parseShader(const std::string& filepath);
//static unsigned int compileShader(const std::string& source, unsigned int type);
//static unsigned int createShader(const std::string& vertexShader, const std::string& fragShader);

int main(int argc, char** argv){
    
    // Main window provided by GLFW (not created yet)
    GLFWwindow* win;

    // Checks if GLFW was initialized
    if(!glfwInit()) exit(-1);
    
    // Actually create the window (width, height, title, resizable, idk)
    win = glfwCreateWindow(640, 640, "It works!", NULL, NULL);
    if(!win){
        glfwTerminate();
        std::exit(-1);
    }

    // Select the window
    glfwMakeContextCurrent(win);
    
    // Match the framerate to the monitor refresh rate
    glfwSwapInterval(1);
    
    // Remeber to initialize glew AFTER selecting the context
    if(glewInit() != GLEW_OK){
        std::cout << "Error!\n";
        std::exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLCALL(std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl);
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glEnable(GL_CULL_FACE));
    /*
    // Vertices temp data
    float positions[] = {
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
    }; // count = 8 * 3 = 24
    */

    std::string grid = 
        "0101"
        "1010"
        "0101"
        "1010"
    
        "1010"
        "0101"
        "1010"
        "0101"

        "0101"
        "1010"
        "0101"
        "1010"

        "1010"
        "0101"
        "1010"
        "0101";
    
    // grid template, width, height, depth, xSize, ySize, zSize
    SuperCube sc = SuperCube(grid.c_str(), 1, 4, 16, 4, 4, 4);

    std::array<glm::vec3, 8> positions = {
        glm::vec3( -0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f)
    };
    
    // Index buffer positions
    unsigned int indices[] = {
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
/*
    Mesh mesh;

    if(argc > 1){
        std::string objFile = argv[1];
        mesh.parseFile(objFile);
    } else {
        mesh.parseFile("./models/cube.obj");
    }
*/
    Renderer renderer;

    // Model * View * Projection Matrix ( OpenGL requires it to be in reverse )
    // Model: The object to render with Translations, Rotations, and Scaling
    // View: The Camera
    // Projection: The conversion of 3D to 2D coordinates 
    
    glm::mat4 proj = glm::perspective(glm::radians(60.f), 640.f/640.f, 0.01f, 100.f);
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f));
    glm::vec3 trans(0.f);
    glm::vec3 scaler(1.f);
    glm::vec3 uColor(1.f);
    glm::vec3 uLightDir(1.f);
    float scaleFactor = 1;
    
    /* Vertex array object
     * Links to the currently bound buffer when you call glVertexAttribPointer
     * Allows you to simply bind the vao and ibo without binding everything else all the time
     **/

    //unsigned int vao;
    //GLCALL(glGenVertexArrays(1, &vao));
    //GLCALL(glBindVertexArray(vao));

    // Buffer is the ID for the data
    VertexBuffer vb(sc.getVert(), sc.getCount() * 3 * sizeof(float));
    VertexBufferLayout vbl;
    vbl.Push<float>(3);

    /* Attributes are the different sections of your data
     * i.e. positions, colors, normals
     * Select the index for the Attribute
     **/
    //GLCALL(glEnableVertexAttribArray(0)); 
    
    /* Specify how this Attribute should be interpreted
     * Create an index that can be selected,
     * Size is the dimension of vertices i.e Vec2 or Vec3 (only allows up to 4)
     * Type is the data type of each vertex
     * Normalized makes sure the value is between -1 and 1
     * Stride size of each vertex
     * Pointer is where this index starts
     * (index, size, type, normalized, stride, pointer)
     **/
    //GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0));
    
    VertexArray va;
    va.addBuffer(vb, vbl);

    // Generate and populate Index Buffer
    // ibo = Index Buffer Object
    IndexBuffer ib(sc.getInd(), sc.getICount());
    
    // Read and return the shader files as strings
    //ShaderSources sources = parseShader("res/shaders/base.shader");
    
    // Returns the program id for both shaders
    //unsigned int shaderProgram = createShader(sources.vertexShader, sources.fragShader);

    // Select the shader (bind)
    //GLCALL(glUseProgram(shaderProgram));
    
    // Link to the color variable in our shaders
    //GLCALL(int location = glGetUniformLocation(shaderProgram, "u_Color"));
    //ASSERT(location != -1);

    // Update the shader variable
    //GLCALL(glUniform4f(location, 0.3f, 0.5f, 1.0f, 1.0f));

    Shader shader("res/shaders/base.shader");
    shader.Bind();
    shader.setUniform4f("u_Color", 0.3f, 0.5f, 1.0f, 1.0f);

    float colorValue = 0.05f;
    float i = 0.f;

    // Function keyCallback is called when input is detected
    glfwSetKeyCallback(win, keyCallback); 

    // ImGUI initialization
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui::StyleColorsDark();

    while(!glfwWindowShouldClose(win)){

        float ratio;
        int width, height;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glfwGetFramebufferSize(win, &width, &height);
        ratio = width/(float)height;

        // How much of the window is seen
        GLCALL(glViewport(0, 0, width, height));
        
        // Clear screen to grey
        //GLCALL(glClear(GL_COLOR_BUFFER_BIT));
        //GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1));
        renderer.clear();

        // Select our index buffer
        //ib.Bind();

        /**
        positions[0] = colorValue -0.5;
        positions[2] = colorValue +0.5;
        positions[4] = colorValue +0.5;
        positions[6] = colorValue -0.5;
        */

        //va.Bind();
        //vb.setData(positions, 4 * 2 * sizeof(float));
        
        glm::mat4 model(1.f);
        
        model = glm::scale(model, scaleFactor * scaler);
        model = glm::rotate(model, glm::radians(trans[0]), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(trans[1]), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(trans[2]), glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 MVP = proj * view * model;
        
        
        // Update color uniform
        //GLCALL(glUniform4f(location, colorValue, 0.5f, 0.5f, 1.0f));
        shader.setUniform4f("u_Color", uColor[0], uColor[1], uColor[2], 1.0f);
        shader.setUniform3f("u_LightDir", uLightDir);
        shader.setUniformMat4f("u_MVP", MVP);
        shader.setUniformMat4f("u_Model", model);
        shader.setUniformMat4f("u_View", view);

        // Update color value
        //colorValue = std::abs(std::sin(i+=0.01));

        // Draw the currently bound buffer
        // Draw elements is used for drawing with indices instead of raw positions
        // nullptr is used instead of ibo because its already bound
        
        //GLCALL(glDrawElements(GL_LINES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
        //GLCALL(glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, nullptr));
        
        //shader.setUniform4f("u_Color", 0.5f, 0.5f, 1.0f, 1.0f);
        renderer.drawTris(va, ib, shader);
        
        //shader.setUniform4f("u_Color", 0.f, 0.f, 0.f, 1.f);
        //GLCALL(glDrawElements(GL_LINES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
        
        {
            ImGui::Begin("Debug Window");
            ImGui::Text("Position Sliders");
            ImGui::SliderFloat3("Rotation XYZ", &trans[0], -180.f, 180.f);
            ImGui::SliderFloat3("Color RGB", &uColor[0], 0.f, 1.f);
            ImGui::SliderFloat3("Light Direction Vector XYZ", &uLightDir[0], -1.f, 1.f);
            ImGui::SliderFloat("Scale", &scaleFactor, 0.f, 2.f);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(win);
        glfwPollEvents();
    
    }

    // Clean up 
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // GLCALL(glDeleteProgram(shaderProgram));
    glfwDestroyWindow(win);
    glfwTerminate();
}

void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_Q && action == GLFW_PRESS){
        std::cout << "Close Program\n";
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

/*
unsigned int compileShader(const std::string& source, unsigned int type){
    
    GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    
    // Select the shader address and compile
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));
    
    // Error handling
    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    
    // Something went wrong
    if(result == GL_FALSE){
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        
        // message must be a const length
        // alloca() dynamically allocates this memory on the stack
        char* message = (char*)alloca(length * sizeof(char));
        GLCALL(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile shader!\n" << message << std::endl;
        
        // Delete shader because it failed
        GLCALL(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int createShader(const std::string& vertexShader, const std::string& fragShader){
    
    // id of the current program
    GLCALL(unsigned int program = glCreateProgram());
    
    // vs and fs hold the id to their shaders
    unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = compileShader(fragShader, GL_FRAGMENT_SHADER);

    // Give the shaders to our program
    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    
    // Make sure our program doesn't have any problems
    GLCALL(glLinkProgram(program));
    GLCALL(glValidateProgram(program));

    // Remove shaders after saving the program
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;

}

ShaderSources parseShader(const std::string& filepath){

    std::ifstream inFile(filepath);
    if(!inFile) std::cout << "Failed to open '" << filepath << "'\n"; 

    std::string line;
    std::stringstream ss[2];
    
    ShaderType type = ShaderType::NONE;

    while(getline(inFile, line)){
        if(line.find("#shader") != std::string::npos){
            if(line.find("vertex") != std::string::npos){
                type = ShaderType::VERTEX;
            } else if(line.find("fragment") != std::string::npos){
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << std::endl;
        }
    }

    inFile.close();

    return { ss[ShaderType::VERTEX].str(), ss[ShaderType::FRAGMENT].str() };

}
*/
