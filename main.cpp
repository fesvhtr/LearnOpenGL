#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <camera.h>
#include <iostream>
#include <stb_image.h>
#include <texture.h>
#include <global.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
//glm::mat4 transform();
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);
void drawLightCube(Shader lt_shader,unsigned int VAO);
void drawWindow(Shader window_shader,unsigned int VAO,unsigned int map);
void drawPoints(Shader point_shader,unsigned int VAO);
// global settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//camera setting
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
glm::vec3 lightPos(1.0f,1.0f,3.0f);
int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //config global opengl test-----------------------------------
    //Face Culling----------------------------------------
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CW);
    //MSAA--------------------------------
    glEnable(GL_MULTISAMPLE);

    //depth test------------------------------------------
    glEnable(GL_DEPTH_TEST);
    //set z-buffer type
    glDepthFunc(GL_LESS);

    //stencil buffer------------------------------------
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    //blend config----------------------------------------
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile our shader program
    Shader ourShader("D:/projects for lessons/CGchuyan/texture.vs",
                     "D:/projects for lessons/CGchuyan/texture.fs");
    Shader lt_shader("D:/projects for lessons/CGchuyan/lt_shader.vs",
                        "D:/projects for lessons/CGchuyan/lt_shader.fs");
    Shader edge_shader("D:/projects for lessons/CGchuyan/texture.vs",
                       "D:/projects for lessons/CGchuyan/shaderSingleColor.fs");
    Shader window_shader("D:/projects for lessons/CGchuyan/window.vs",
                         "D:/projects for lessons/CGchuyan/window.fs");
    Shader sky_shader("D:/projects for lessons/CGchuyan/skybox.vs",
                      "D:/projects for lessons/CGchuyan/skybox.fs");


    //set a framebuffer
//    unsigned int FBO;
//    glGenFramebuffers(1,&FBO);
//    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
//    //bind a texture to the FBO
//    unsigned int texColorBuffer;
//    glGenTextures(1, &texColorBuffer);
//    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    //attach texture to the FBO
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
//    //set a renderbufferObject
//    unsigned int RBO;
//    glGenRenderbuffers(1, &RBO);
//    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
//    //attach RBO to FBO
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//    //final check FBO complete
//    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //set cubeVAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    //bind VAO,VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //set light VAO
    unsigned int lightVAO,lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //set transparent window VAO,VBO
    unsigned int windowVAO,windowVBO;
    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);
    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    unsigned int skyVAO,skyVBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //set uniform buffer object
    unsigned int UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
    //set textures
//    TEXTURE texture1("D:/projects for lessons/CGchuyan/zzface.jpg");
//    TEXTURE texture2("D:/projects for lessons/CGchuyan/container.jpg");
    //load 2D textures
    unsigned int diffuse_map1 = loadTexture("D:/projects for lessons/CGchuyan/medo.jpg");
    unsigned int diffuse_map2 = loadTexture("D:/projects for lessons/CGchuyan/container.jpg");
    unsigned int window_map = loadTexture("D:/projects for lessons/CGchuyan/window.png");
    //load skybox
    unsigned int cubemapTexture = loadCubemap(faces);
    //set shader int for each TEXTURE N
    ourShader.use();
    ourShader.setInt("material_zzface.diffuse",0);
    ourShader.setInt("material_container.diffuse",1);
    window_shader.use();
    window_shader.setInt("texture_window",2);
    sky_shader.use();
    sky_shader.setInt("skybox",0);
    //ourShader.setInt("material.specular",1);
    while (!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //clear depth each render -else remain last z
        //clear color buffer
        //clear stencil buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //avoid depth buffer clear
        //glDepthMask(GL_FALSE);
        //init view and projection matrix
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        //get view and projection which not change
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //render skybox first always---------------------------------------
        glStencilMask(0x00);
        glDepthMask(GL_FALSE);
        glm::mat4 sky_view = glm::mat4(glm::mat3(view));
        sky_shader.use();
        sky_shader.setMat4("sky_view",sky_view);
        sky_shader.setMat4("sky_projection",projection);
        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        //render light cube---------------------------------------------
        glStencilMask(0x00);
        drawLightCube(lt_shader,lightVAO);
        //render cubes--------------------------------------------------------

        ourShader.use();
        //set cube shader
        ourShader.setVec3("viewPos",camera.Position);
        ourShader.setVec3("light.position",lightPos);
        ourShader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
        ourShader.setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
        ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        //set textures
        ourShader.setVec3("material_zzface.specular", 0.5f, 0.5f, 0.5f);
        ourShader.setFloat("material_zzface.shininess", 64.0f);
        ourShader.setVec3("material_container.specular", 0.5f, 0.5f, 0.5f);
        ourShader.setFloat("material_container.shininess", 64.0f);
        //1st. render pass, draw objects as normal, writing to the stencil buffer
        //set all stencil 1
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        //texture active and bind
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuse_map1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuse_map2);
        //set buffer for model in shader for instancing
        glm::mat4 *model_matrices;
        model_matrices = new glm::mat4[10];
        for(unsigned int j = 0; j < 10; j++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,cubePositions[j]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            model_matrices[j] = model;
        }
        //matrix buffer for model
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 10 * sizeof(glm::mat4), &model_matrices[0], GL_STATIC_DRAW);
        for (unsigned int i = 0; i < 10; i++)
        {
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
            glBindVertexArray(0);
        }
        // init model and trans each render
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model,cubePositions[i]);
        //calculate model
        float angle = 20.0f;
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 10);
        //2nd - edge
//            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//            glStencilMask(0x00);
//            //glDisable(GL_DEPTH_TEST);
//            edge_shader.use();
//            model= glm::mat4(1.0f);
//            trans = glm::mat4(1.0f);
//            //calculate model
//            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
//            //model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
//            model = glm::scale(model,glm::vec3(1.1f,1.1f,1.1f));
//            trans = projection * view * model;
//            edge_shader.setMat4("model",model);
//            edge_shader.setMat4("trans", trans);
//            glBindVertexArray(VAO);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//            glStencilMask(0xFF);
        //glEnable(GL_DEPTH_TEST);
        //clear stencil buffer every time - so every cube has own edge
        //else the next cube's stencil value equals 1 - past edge be covered
        //glClear(GL_STENCIL_BUFFER_BIT);

        //render window----------------------------------------------------
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        drawWindow(window_shader,windowVAO,window_map);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // delete all vertex obj
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &windowVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &windowVBO);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}\
//calculate transform matrix
glm::mat4 transform(){
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glm::mat4 model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    trans = projection * view * model;
    return trans;
}
// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
//load 2D texture
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
//        if(idx == 1)
//            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        //way of interpolation
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //Mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
//load 3D texture
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
//draw light cube
void drawLightCube(Shader lt_shader,unsigned int VAO){
    glBindVertexArray(VAO);
    //init all matrices
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 lt_model = glm::mat4(1.0f);
    glm::mat4 lt_trans = glm::mat4(1.0f);
    //get view and projection which not change
    view = camera.GetViewMatrix();
    lt_model = glm::translate(lt_model,lightPos);
    projection = glm::perspective(glm::radians(camera.Zoom),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    lt_trans = projection * view * lt_model;
    lt_shader.use();
    lt_shader.setMat4("lt_trans",lt_trans);
    //draw light cube
    glDrawArrays(GL_TRIANGLES,0,36);
}
//draw window
void drawWindow(Shader window_shader,unsigned int windowVAO,unsigned int map){
    window_shader.use();
    glBindVertexArray(windowVAO);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, map);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 window_model = glm::mat4(1.0f);
    glm::mat4 window_trans = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera.Zoom),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    view = camera.GetViewMatrix();
    window_model = glm::translate(window_model, glm::vec3(0.0f,  0.0f,  1.0f));
    window_model = glm::rotate(window_model,  glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    window_trans = projection * view * window_model;
    window_shader.setMat4("window_trans",window_trans);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
//draw points with geography shader
void drawPoints(Shader point_shader,unsigned int VAO){

}