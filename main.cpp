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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
//glm::mat4 transform();
unsigned int loadTexture(const char *path);
void drawLightCube(Shader lt_shader,unsigned int VAO);
void drawWindow(Shader window_shader,unsigned int VAO,unsigned int map);
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
//light data
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//cube vertex data
float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
//cube position
glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};
float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};
//vegetable data
//std::vector<glm::vec3> vegetation;
//vegetation = {
//
//};
//vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
//vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
//vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
//vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
//vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));
int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
    //depth test------------------------------------------
    glEnable(GL_DEPTH_TEST);
    //set z-buffer type
    glDepthFunc(GL_LESS);
    //glDepthFunc(GL_ALWAYS);
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
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
    //set textures
//    TEXTURE texture1("D:/projects for lessons/CGchuyan/zzface.jpg");
//    TEXTURE texture2("D:/projects for lessons/CGchuyan/container.jpg");
    unsigned int diffuse_map1 = loadTexture("D:/projects for lessons/CGchuyan/zzface.jpg");
    unsigned int diffuse_map2 = loadTexture("D:/projects for lessons/CGchuyan/container.jpg");
    unsigned int window_map = loadTexture("D:/projects for lessons/CGchuyan/window.png");
    //set shader int for each TEXTURE N
    ourShader.use();
    ourShader.setInt("material_zzface.diffuse",0);
    ourShader.setInt("material_container.diffuse",1);
    window_shader.use();
    window_shader.setInt("texture_window",2);
    //ourShader.setInt("material.specular",1);
    while (!glfwWindowShouldClose(window))
    {
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
        // Active and bind Textures
//        glActiveTexture(GL_TEXTURE0);
//        glActiveTexture(GL_TEXTURE1);

        //init view and projection matrix
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        //get view and projection which not change
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //render light cube---------------------------------------------
        glStencilMask(0x00);
        drawLightCube(lt_shader,VAO);
        //render window----------------------------------------------------
        //drawWindow(window_shader,windowVAO,window_map);
        //render cubes--------------------------------------------------------
        for (unsigned int i = 0; i < 10; i++)
        {
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
            // init model and trans each render
            glm::mat4 model= glm::mat4(1.0f);
            glm::mat4 trans = glm::mat4(1.0f);
            //calculate model
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i+1);
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            model = glm::rotate(model,  glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            trans = projection * view * model;
            ourShader.setMat4("model",model);
            ourShader.setMat4("trans", trans);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //2st
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            //glDisable(GL_DEPTH_TEST);
            edge_shader.use();
            model= glm::mat4(1.0f);
            trans = glm::mat4(1.0f);
            //calculate model
            model = glm::translate(model, cubePositions[i]);
            //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            model = glm::scale(model,glm::vec3(1.1f,1.1f,1.1f));
            trans = projection * view * model;
            edge_shader.setMat4("model",model);
            edge_shader.setMat4("trans", trans);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glStencilMask(0xFF);
            //glEnable(GL_DEPTH_TEST);
            //clear stencil buffer every time - so every cube has own edge
            //else the next cube's stencil value equals 1 - past edge be covered
            glClear(GL_STENCIL_BUFFER_BIT);
        }
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
//load texture
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
//draw light cube
void drawLightCube(Shader lt_shader,unsigned int VAO){
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
    glBindVertexArray(VAO);
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