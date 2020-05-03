#include "Renderer.hpp"

void GLClearErrors(){
    // Remove all previous error messages if any
    while(GLenum error = glGetError());
}

bool GLLogCall(const char* functionName, const char* filepath, int lineNumber){
    // Output error messages if any and their location
    while(GLenum error = glGetError()){
        std::cout << "[OpenGL Error] - (" << error << ") '" 
                  << functionName 
                  << "' In file: '" << filepath 
                  << "' line:" << lineNumber << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer(const int& h, const int& w, const std::string& title){
    std::cout << "Constructor\n";

    // Checks if GLFW was initialized
    if(!glfwInit()) std::exit(-1);
    
    // Actually create the window (width, height, title, resizable, idk)
    this->win = glfwCreateWindow(h, w, title.c_str(), NULL, NULL);
    if(!this->win){
        glfwTerminate();
        std::exit(-1);
    }

    // Select the window
    glfwMakeContextCurrent(this->win);
    
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

    //GLCALL(glfwSetKeyCallback(win, this->onEvent));

    this->m_VertData = new Vertex[this->MAXSIZE];
    this->m_IndData  = new unsigned int[this->MAXSIZE * 3];
    this->m_Count = 0;
    this->m_ICount = 0;

    this->vb.create(this->m_VertData, this->MAXSIZE * sizeof(Vertex));
    this->vbl.Push<float>(3);
    this->vbl.Push<float>(3);

    this->va.create();
    this->va.addBuffer(this->vb, this->vbl);

    this->ib.create(this->m_IndData, this->MAXSIZE * 3);
    this->shader.create("./res/shaders/base.shader");

}

Renderer::~Renderer(){
    delete[] this->m_VertData;
    delete[] this->m_IndData;
}

void Renderer::init(){
    //this->m_UserUpdate = userUpdate;
    
    // Function keyCallback is called when input is detected
    //glfwSetKeyCallback(win, keyCallback);

    std::cout << "Init\n";

    this->onCreate();
    this->dTime = glfwGetTime();
    this->coreUpdate();
}

void Renderer::coreUpdate(){

    std::cout << "Core\n";

    // Model * View * Projection Matrix ( OpenGL requires it to be in reverse )
    // Model: The object to render with Translations, Rotations, and Scaling
    // View: The Camera
    // Projection: The conversion of 3D to 2D coordinates 

    glm::mat4 proj = glm::perspective(glm::radians(60.f), 640.f/640.f, 0.01f, 100.f);
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f));

    glm::vec3 trans(30.f, -45.f, 0.f);
    glm::vec3 scaler(0.3f);
    glm::vec3 uColor(1.f);
    glm::vec3 uLightDir(1.f);
    
    float scaleFactor = 1.f;

    // ImGUI initialization
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui::StyleColorsDark();

    while(!glfwWindowShouldClose(this->win)){

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float ratio;
        int width, height;
        glfwGetFramebufferSize(this->win, &width, &height);
        ratio = width/(float)height;

        // How much of the window is seen
        GLCALL(glViewport(0, 0, width, height));

        this->clear();

        glm::mat4 model(1.f);
        
        model = glm::scale(model, scaleFactor * scaler);
        model = glm::rotate(model, glm::radians(trans[0]), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(trans[1]), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(trans[2]), glm::vec3(0.f, 0.f, 1.f));
        glm::mat4 MVP = proj * view * model;
        
        // Update color uniform
        this->shader.Bind();
        this->shader.setUniform4f("u_Color", uColor[0], uColor[1], uColor[2], 1.0f);
        this->shader.setUniform3f("u_LightDir", uLightDir);
        this->shader.setUniformMat4f("u_MVP", MVP);
        this->shader.setUniformMat4f("u_Model", model);
        this->shader.setUniformMat4f("u_View", view);

        this->onUpdate(glfwGetTime() - this->dTime);
        this->dTime = glfwGetTime();
        this->flush();

        {   // ImGui Debug window 
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

        glfwSwapBuffers(this->win);
        glfwPollEvents();
        
        //glfwSetWindowShouldClose(win, GLFW_TRUE);
    }

    // Clean up ImGui Instance
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // GLCALL(glDeleteProgram(shaderProgram));
    glfwDestroyWindow(this->win);
    glfwTerminate();

}

void Renderer::onCreate(){}

void Renderer::onUpdate(double dt){}

//void Renderer::onEvent(GLFWwindow* win, int key, int scancode, int action, int mods){}

void Renderer::clear() const {
    // Clear screen to grey
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1));
}

void Renderer::drawMesh(Mesh& mesh){
    //std::cout << "Draw\n";

    if(mesh.getCount() + this->m_Count > this->MAXSIZE || mesh.getICount() + this->m_ICount > this->MAXSIZE){
        //std::cout << "[Flush early] - "
        //          << "Count: "  << this->m_Count
        //          << " | ICount: " << this->m_ICount << std::endl;
        this->flush();
    }

    // Test with memcpy() later

    for(unsigned long i = 0; i < mesh.getCount(); i++){
        this->m_VertData[this->m_Count + i] = mesh.getVert()[i];
    }

    for(unsigned long i = 0; i < mesh.getICount(); i++){
        this->m_IndData[this->m_ICount + i] = mesh.getInd()[i] + this->m_Count;
    }

    // Update sizes
    this->m_Count  += mesh.getCount();
    this->m_ICount += mesh.getICount();
}

void Renderer::flush(){
    //std::cout << "Flush\n";

    //std::cout << "Count: "  << this->m_Count  << std::endl;
    //std::cout << "ICount: " << this->m_ICount << std::endl;

    this->va.Bind();
    this->vb.setData(this->m_VertData, this->m_Count * sizeof(Vertex));

    this->ib.Bind();
    this->ib.setData(this->m_IndData, this->m_ICount);

    this->shader.Bind();

    // Draw everything batched together
    GLCALL(glDrawElements(GL_TRIANGLES, this->ib.getCount(), GL_UNSIGNED_INT, nullptr));

    // Reset all data
    
    this->vb.setData(this->m_VertData, 0);
    this->ib.setData(this->m_IndData, 0);
    
    this->m_Count  = 0;
    this->m_ICount = 0;
}
