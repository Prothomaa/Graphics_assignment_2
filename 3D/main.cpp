//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void Fan(Shader ourShader, glm::mat4 moveMatrix);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 45.0;
float rotateAngle_Y = 45.0;
float rotateAngle_Z = 45.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float rotateLevel = 0.0;
float rotate_Now = 0.0;

// camera
Camera camera(glm::vec3(1.5f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
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
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.3f, 0.8f, 0.5f,
        0.5f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f,
        0.5f, 0.5f, 0.0f, 0.2f, 0.7f, 0.3f,
        0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.8f,
        0.0f, 0.0f, 0.5f, 0.8f, 0.3f, 0.6f,
        0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.6f,
        0.0f, 0.5f, 0.5f, 0.7f, 0.5f, 0.4f
    };*/
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f
    };*/
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };*/
    /*float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };*/
    // world space positions of our cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);

        // Modelling Transformation
        /*
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        */
        //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));

        //khat o 2ta pillow
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, -0.85, -1.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 3.0, 0.5));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.2f, 0.4f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
        translateMatrix2 = glm::translate(identityMatrix, glm::vec3(1.05, -0.7, -1.0));
        rotateXMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix2 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix2 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(0.8, 0.5, 0.5));
        model2 = translateMatrix2 * rotateXMatrix2 * rotateYMatrix2 * rotateZMatrix2 * scaleMatrix2;
        ourShader.setMat4("model", model2);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.6f, 0.8f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix3, rotateXMatrix3, rotateYMatrix3, rotateZMatrix3, scaleMatrix3, model3;
        translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.58, -0.7, -1.0));
        rotateXMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix3 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix3 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix3 = glm::scale(identityMatrix, glm::vec3(0.8, 0.5, 0.5));
        model3 = translateMatrix3 * rotateXMatrix3 * rotateYMatrix3 * rotateZMatrix3 * scaleMatrix3;
        ourShader.setMat4("model", model3);

        glBindVertexArray(VAO);
        //ourShader.setVec3("COLOR", glm::vec3(0.48f, 0.35f, 0.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //almira

        glm::mat4 translateMatrix4, rotateXMatrix4, rotateYMatrix4, rotateZMatrix4, scaleMatrix4, model4;
        translateMatrix4 = glm::translate(identityMatrix, glm::vec3(-1.9, 0.9, -1.0));
        rotateXMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix4 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix4 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.2, 2.0, 4.0));
        model4 = translateMatrix4 * rotateXMatrix4 * rotateYMatrix4 * rotateZMatrix4 * scaleMatrix4;
        ourShader.setMat4("model", model4);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix19, rotateXMatrix19, rotateYMatrix19, rotateZMatrix19, scaleMatrix19, model19;
        translateMatrix19 = glm::translate(identityMatrix, glm::vec3(-1.9, 0.9, -1.0));
        rotateXMatrix19 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix19 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix19 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix19 = glm::scale(identityMatrix, glm::vec3(1.0, 2.0, 0.2));
        model19 = translateMatrix19 * rotateXMatrix19 * rotateYMatrix19 * rotateZMatrix19 * scaleMatrix19;
        ourShader.setMat4("model", model19);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.2f, 0.4f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix20, rotateXMatrix20, rotateYMatrix20, rotateZMatrix20, scaleMatrix20, model20;
        translateMatrix20 = glm::translate(identityMatrix, glm::vec3(-1.9, 0.5, -1.0));
        rotateXMatrix20 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix20 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix20 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix20 = glm::scale(identityMatrix, glm::vec3(1.0, 2.0, 0.2));
        model20 = translateMatrix20 * rotateXMatrix20 * rotateYMatrix20 * rotateZMatrix20 * scaleMatrix20;
        ourShader.setMat4("model", model20);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.2f, 0.4f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix21, rotateXMatrix21, rotateYMatrix21, rotateZMatrix21, scaleMatrix21, model21;
        translateMatrix21 = glm::translate(identityMatrix, glm::vec3(-1.9, 0.05, -1.0));
        rotateXMatrix21 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix21 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix21 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix21 = glm::scale(identityMatrix, glm::vec3(1.0, 2.0, 0.2));
        model21 = translateMatrix21 * rotateXMatrix21 * rotateYMatrix21 * rotateZMatrix21 * scaleMatrix21;
        ourShader.setMat4("model", model21);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.2f, 0.4f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix22, rotateXMatrix22, rotateYMatrix22, rotateZMatrix22, scaleMatrix22, model22;
        translateMatrix22 = glm::translate(identityMatrix, glm::vec3(-1.9, -0.45, -1.0));
        rotateXMatrix22 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix22 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix22 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix22 = glm::scale(identityMatrix, glm::vec3(1.0, 2.0, 0.2));
        model22 = translateMatrix22 * rotateXMatrix22 * rotateYMatrix22 * rotateZMatrix22 * scaleMatrix22;
        ourShader.setMat4("model", model22);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.2f, 0.4f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix23, rotateXMatrix23, rotateYMatrix23, rotateZMatrix23, scaleMatrix23, model23;
        translateMatrix23 = glm::translate(identityMatrix, glm::vec3(-1.9, -0.9, -1.0));
        rotateXMatrix23 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix23 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix23 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix23 = glm::scale(identityMatrix, glm::vec3(1.0, 2.0, 0.2));
        model23 = translateMatrix23 * rotateXMatrix23 * rotateYMatrix23 * rotateZMatrix23 * scaleMatrix23;
        ourShader.setMat4("model", model23);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.2f, 0.4f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix24, rotateXMatrix24, rotateYMatrix24, rotateZMatrix24, scaleMatrix24, model24;
        translateMatrix24 = glm::translate(identityMatrix, glm::vec3(-1.9, 0.9, -1.0));
        rotateXMatrix24 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix24 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix24 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix24 = glm::scale(identityMatrix, glm::vec3(1.0, 0.2, 4.0));
        model24 = translateMatrix24 * rotateXMatrix24 * rotateYMatrix24 * rotateZMatrix24 * scaleMatrix24;
        ourShader.setMat4("model", model24);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.2f, 0.4f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix25, rotateXMatrix25, rotateYMatrix25, rotateZMatrix25, scaleMatrix25, model25;
        translateMatrix25 = glm::translate(identityMatrix, glm::vec3(-1.9, 0.9, 0.0));
        rotateXMatrix25 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix25 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix25 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix25 = glm::scale(identityMatrix, glm::vec3(1.0, 0.2, 4.0));
        model25 = translateMatrix25 * rotateXMatrix25 * rotateYMatrix25 * rotateZMatrix25 * scaleMatrix25;
        ourShader.setMat4("model", model25);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.2f, 0.4f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
       
        //table o 4ta pa

        glm::mat4 translateMatrix5, rotateXMatrix5, rotateYMatrix5, rotateZMatrix5, scaleMatrix5, model5;
        translateMatrix5 = glm::translate(identityMatrix, glm::vec3(-1.9, -0.1, 1.0));
        rotateXMatrix5 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix5 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix5 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix5 = glm::scale(identityMatrix, glm::vec3(1.2, 2.0, 0.2));
        model5 = translateMatrix5 * rotateXMatrix5 * rotateYMatrix5 * rotateZMatrix5 * scaleMatrix5;
        ourShader.setMat4("model", model5);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.6f, 0.35f, 0.2f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix6, rotateXMatrix6, rotateYMatrix6, rotateZMatrix6, scaleMatrix6, model6;
        translateMatrix6 = glm::translate(identityMatrix, glm::vec3(-1.8, -0.1, 1.0));
        rotateXMatrix6 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix6 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix6 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix6 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
        model6 = translateMatrix6 * rotateXMatrix6 * rotateYMatrix6 * rotateZMatrix6 * scaleMatrix6;
        ourShader.setMat4("model", model6);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix7, rotateXMatrix7, rotateYMatrix7, rotateZMatrix7, scaleMatrix7, model7;
        translateMatrix7 = glm::translate(identityMatrix, glm::vec3(-1.4, -0.1, 1.0));
        rotateXMatrix7 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix7 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix7 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix7 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
        model7 = translateMatrix7 * rotateXMatrix7 * rotateYMatrix7 * rotateZMatrix7 * scaleMatrix7;
        ourShader.setMat4("model", model7);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix8, rotateXMatrix8, rotateYMatrix8, rotateZMatrix8, scaleMatrix8, model8;
        translateMatrix8 = glm::translate(identityMatrix, glm::vec3(-1.8, -0.1, 1.9));
        rotateXMatrix8 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix8 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix8 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix8 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
        model8 = translateMatrix8 * rotateXMatrix8 * rotateYMatrix8 * rotateZMatrix8 * scaleMatrix8;
        ourShader.setMat4("model", model8);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix9, rotateXMatrix9, rotateYMatrix9, rotateZMatrix9, scaleMatrix9, model9;
        translateMatrix9 = glm::translate(identityMatrix, glm::vec3(-1.4, -0.1, 1.9));
        rotateXMatrix9 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix9 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix9 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix9 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
        model9 = translateMatrix9 * rotateXMatrix9 * rotateYMatrix9 * rotateZMatrix9 * scaleMatrix9;
        ourShader.setMat4("model", model9);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //chair

        glm::mat4 translateMatrix10, rotateXMatrix10, rotateYMatrix10, rotateZMatrix10, scaleMatrix10, model10;
        translateMatrix10 = glm::translate(identityMatrix, glm::vec3(-0.9, -0.6, 1.0));
        rotateXMatrix10 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix10 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix10 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix10 = glm::scale(identityMatrix, glm::vec3(0.9, 0.9, 0.1));
        model10 = translateMatrix10 * rotateXMatrix10 * rotateYMatrix10 * rotateZMatrix10 * scaleMatrix10;
        ourShader.setMat4("model", model10);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.8f, 0.5f, 0.2f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix11, rotateXMatrix11, rotateYMatrix11, rotateZMatrix11, scaleMatrix11, model11;
        translateMatrix11 = glm::translate(identityMatrix, glm::vec3(-0.8, -0.6, 1.0));
        rotateXMatrix11 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix11 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix11 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix11 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 1.0));
        model11 = translateMatrix11 * rotateXMatrix11 * rotateYMatrix11 * rotateZMatrix11 * scaleMatrix11;
        ourShader.setMat4("model", model11);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix12, rotateXMatrix12, rotateYMatrix12, rotateZMatrix12, scaleMatrix12, model12;
        translateMatrix12 = glm::translate(identityMatrix, glm::vec3(-0.55, -0.6, 1.0));
        rotateXMatrix12 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix12 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix12 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix12 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 1.0));
        model12 = translateMatrix12 * rotateXMatrix12 * rotateYMatrix12 * rotateZMatrix12 * scaleMatrix12;
        ourShader.setMat4("model", model12);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix13, rotateXMatrix13, rotateYMatrix13, rotateZMatrix13, scaleMatrix13, model13;
        translateMatrix13 = glm::translate(identityMatrix, glm::vec3(-0.55, -0.6, 1.4));
        rotateXMatrix13 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix13 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix13 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix13 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 1.0));
        model13 = translateMatrix13 * rotateXMatrix13 * rotateYMatrix13 * rotateZMatrix13 * scaleMatrix13;
        ourShader.setMat4("model", model13);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix14, rotateXMatrix14, rotateYMatrix14, rotateZMatrix14, scaleMatrix14, model14;
        translateMatrix14 = glm::translate(identityMatrix, glm::vec3(-0.8, -0.6, 1.4));
        rotateXMatrix14 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix14 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix14 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix14 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 1.0));
        model14 = translateMatrix14 * rotateXMatrix14 * rotateYMatrix14 * rotateZMatrix14 * scaleMatrix14;
        ourShader.setMat4("model", model14);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix15, rotateXMatrix15, rotateYMatrix15, rotateZMatrix15, scaleMatrix15, model15;
        translateMatrix15 = glm::translate(identityMatrix, glm::vec3(-0.55, 0.0, 1.4));
        rotateXMatrix15 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix15 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix15 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix15 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
        model15 = translateMatrix15 * rotateXMatrix15 * rotateYMatrix15 * rotateZMatrix15 * scaleMatrix15;
        ourShader.setMat4("model", model15);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix16, rotateXMatrix16, rotateYMatrix16, rotateZMatrix16, scaleMatrix16, model16;
        translateMatrix16 = glm::translate(identityMatrix, glm::vec3(-0.55, 0.0, 1.0));
        rotateXMatrix16 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix16 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix16 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix16 = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
        model16 = translateMatrix16 * rotateXMatrix16 * rotateYMatrix16 * rotateZMatrix16 * scaleMatrix16;
        ourShader.setMat4("model", model16);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix17, rotateXMatrix17, rotateYMatrix17, rotateZMatrix17, scaleMatrix17, model17;
        translateMatrix17 = glm::translate(identityMatrix, glm::vec3(-0.55, -0.0, 1.0));
        rotateXMatrix17 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix17 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix17 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix17 = glm::scale(identityMatrix, glm::vec3(0.1, 0.9, 0.06));
        model17 = translateMatrix17 * rotateXMatrix17 * rotateYMatrix17 * rotateZMatrix17 * scaleMatrix17;
        ourShader.setMat4("model", model17);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix18, rotateXMatrix18, rotateYMatrix18, rotateZMatrix18, scaleMatrix18, model18;
        translateMatrix18 = glm::translate(identityMatrix, glm::vec3(-0.55, -0.1, 1.0));
        rotateXMatrix18 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix18 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix18 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix18 = glm::scale(identityMatrix, glm::vec3(0.1, 0.9, 0.06));
        model18 = translateMatrix18 * rotateXMatrix18 * rotateYMatrix18 * rotateZMatrix18 * scaleMatrix18;
        ourShader.setMat4("model", model18);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //floor
        glm::mat4 translateMatrix26, rotateXMatrix26, rotateYMatrix26, rotateZMatrix26, scaleMatrix26, model26;
        translateMatrix26 = glm::translate(identityMatrix, glm::vec3(-2.4, -1.1, -2.0));
        rotateXMatrix26 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix26 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix26 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix26 = glm::scale(identityMatrix, glm::vec3(9.0, 8.0, 0.2));
        model26 = translateMatrix26 * rotateXMatrix26 * rotateYMatrix26 * rotateZMatrix26 * scaleMatrix26;
        ourShader.setMat4("model", model26);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.9f, 0.9f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix27, rotateXMatrix27, rotateYMatrix27, rotateZMatrix27, scaleMatrix27, model27;
        translateMatrix27 = glm::translate(identityMatrix, glm::vec3(-2.4, 0.9, -2.0));
        rotateXMatrix27 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix27 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix27 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix27 = glm::scale(identityMatrix, glm::vec3(0.3, 8.0, 4.0));
        model27 = translateMatrix27 * rotateXMatrix27 * rotateYMatrix27 * rotateZMatrix27 * scaleMatrix27;
        ourShader.setMat4("model", model27);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.9f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix28, rotateXMatrix28, rotateYMatrix28, rotateZMatrix28, scaleMatrix28, model28;
        translateMatrix28 = glm::translate(identityMatrix, glm::vec3(1.95, 0.9, -2.0));
        rotateXMatrix28 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix28 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix28 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix28 = glm::scale(identityMatrix, glm::vec3(0.3, 8.0, 4.0));
        model28 = translateMatrix28 * rotateXMatrix28 * rotateYMatrix28 * rotateZMatrix28 * scaleMatrix28;
        ourShader.setMat4("model", model28);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.9f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix29, rotateXMatrix29, rotateYMatrix29, rotateZMatrix29, scaleMatrix29, model29;
        translateMatrix29 = glm::translate(identityMatrix, glm::vec3(-2.3, -0.6, -2.0));
        rotateXMatrix29 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix29 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix29 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix29 = glm::scale(identityMatrix, glm::vec3(8.8, 0.3, 1.0));
        model29 = translateMatrix29 * rotateXMatrix29 * rotateYMatrix29 * rotateZMatrix29 * scaleMatrix29;
        ourShader.setMat4("model", model29);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.0f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix30, rotateXMatrix30, rotateYMatrix30, rotateZMatrix30, scaleMatrix30, model30;
        translateMatrix30 = glm::translate(identityMatrix, glm::vec3(-2.3, 0.91, -2.0));
        rotateXMatrix30 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix30 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix30 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix30 = glm::scale(identityMatrix, glm::vec3(8.8, 0.3, 1.0));
        model30 = translateMatrix30 * rotateXMatrix30 * rotateYMatrix30 * rotateZMatrix30 * scaleMatrix30;
        ourShader.setMat4("model", model30);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.0f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix31, rotateXMatrix31, rotateYMatrix31, rotateZMatrix31, scaleMatrix31, model31;
        translateMatrix31 = glm::translate(identityMatrix, glm::vec3(-2.3, 0.8, -2.0));
        rotateXMatrix31 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix31 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix31 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix31 = glm::scale(identityMatrix, glm::vec3(3.0, 0.3, 3.0));
        model31 = translateMatrix31 * rotateXMatrix31 * rotateYMatrix31 * rotateZMatrix31 * scaleMatrix31;
        ourShader.setMat4("model", model31);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.0f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix32, rotateXMatrix32, rotateYMatrix32, rotateZMatrix32, scaleMatrix32, model32;
        translateMatrix32 = glm::translate(identityMatrix, glm::vec3(0.8, 0.6, -2.0));
        rotateXMatrix32 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix32 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix32 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix32 = glm::scale(identityMatrix, glm::vec3(2.5, 0.3, 3.0));
        model32 = translateMatrix32 * rotateXMatrix32 * rotateYMatrix32 * rotateZMatrix32 * scaleMatrix32;
        ourShader.setMat4("model", model32);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.0f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix33, rotateXMatrix33, rotateYMatrix33, rotateZMatrix33, scaleMatrix33, model33;
        translateMatrix33 = glm::translate(identityMatrix, glm::vec3(0.0, 0.6, -2.0));
        rotateXMatrix33 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix33 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix33 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix33 = glm::scale(identityMatrix, glm::vec3(0.1, 0.3, 3.0));
        model33 = translateMatrix33 * rotateXMatrix33 * rotateYMatrix33 * rotateZMatrix33 * scaleMatrix33;
        ourShader.setMat4("model", model33);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.0f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix34, rotateXMatrix34, rotateYMatrix34, rotateZMatrix34, scaleMatrix34, model34;
        translateMatrix34 = glm::translate(identityMatrix, glm::vec3(-2.3, -0.1, -2.0));
        rotateXMatrix34 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix34 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix34 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix34 = glm::scale(identityMatrix, glm::vec3(8.8, 0.1, 0.1));
        model34 = translateMatrix34 * rotateXMatrix34 * rotateYMatrix34 * rotateZMatrix34 * scaleMatrix34;
        ourShader.setMat4("model", model34);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(1.0f, 0.0f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 translateMatrix35, rotateXMatrix35, rotateYMatrix35, rotateZMatrix35, scaleMatrix35, model35;
        translateMatrix35 = glm::translate(identityMatrix, glm::vec3(-2.4, 1.0, -2.0));
        rotateXMatrix35 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix35 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix35 = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix35 = glm::scale(identityMatrix, glm::vec3(9.0, 8.0, 0.2));
        model35 = translateMatrix35 * rotateXMatrix35 * rotateYMatrix35 * rotateZMatrix35 * scaleMatrix35;
        ourShader.setMat4("model", model35);
        glBindVertexArray(VAO);
        ourShader.setVec3("COLOR", glm::vec3(0.9f, 0.9f, 0.9f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //Fan
        glm::mat4 moveMatrix = glm::translate(identityMatrix, glm::vec3(-0.15f, 2.2f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.30f, -3.0f, -0.60f));//,
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        model = rotateYMatrix * scaleMatrix * translateMatrix;
        //moveMatrix = rotateZMatrix * moveMatrix;
        ourShader.setMat4("model", moveMatrix* model);
        ourShader.setVec3("COLOR", glm::vec3(0.48f, 0.35f, 0.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.6f, 0.0f));
        rotate_Now = (rotate_Now + rotateLevel);
        if (rotate_Now == 361.0)
            rotate_Now = 0.0;

        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotate_Now), glm::vec3(0.0f, 1.0f, 0.0f));

        Fan(ourShader, rotateYMatrix * translateMatrix);



        // render boxes
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    ourShader.setMat4("model", model);

        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 1;
        else if (rotateAxis_Y) rotateAngle_Y -= 1;
        else rotateAngle_Z -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.01;
/*
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }*/
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateLevel = rotateLevel + 1.0;
        if (rotateLevel == 3.0)
            rotateLevel = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        lookAtZ -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Fan(Shader ourShader, glm::mat4 moveMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.2f, 0.5f));
    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    model = scaleMatrix * translateMatrix;
    //moveMatrix = rotateZMatrix * moveMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec3("COLOR", glm::vec3(0.0f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));//,translate_X, translate_Y, translate_Z
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.2f, 0.5f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    model = rotateYMatrix * scaleMatrix * translateMatrix;
    //moveMatrix = rotateZMatrix * moveMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.5f));//,
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.2f, 0.5f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    model = rotateYMatrix * scaleMatrix * translateMatrix;
    //moveMatrix = rotateZMatrix * moveMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


}
