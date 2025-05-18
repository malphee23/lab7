#define GLEW_DLL
#define GLFW_DLL

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "shaders.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Model.h"

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 projection;
glm::mat4 view;

glm::mat4 translateOX1 = glm::mat4(1.0);
glm::mat4 translateOX2 = glm::mat4(1.0);
glm::mat4 translateOX3 = glm::mat4(1.0);

float linearOX1 = 0.0;
float linearOX2 = 0.0;
float linearOX3 = 0.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void processInput(GLFWwindow* window) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    //Z
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        float newLinear = linearOX1 - deltaTime;
        if (newLinear >= -0.0615382f) {
            linearOX1 = newLinear;
            translateOX1 = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, linearOX1));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        float newLinear = linearOX1 + deltaTime;
        if (newLinear <= 0.776447f) {
            linearOX1 = newLinear;
            translateOX1 = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, linearOX1));
        }
    }

    //X
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        float newLinear = linearOX2 - deltaTime;
        if (newLinear >= -0.181f) {
            translateOX2 = glm::translate(glm::mat4(1.0), glm::vec3(newLinear, 0, 0));
            linearOX2 = newLinear;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        float newLinear = linearOX2 + deltaTime;
        if (newLinear <= 0.633f) {
            translateOX2 = glm::translate(glm::mat4(1.0), glm::vec3(newLinear, 0, 0));
            linearOX2 = newLinear;
        }
    }

    //Y
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        float newLinear = linearOX3 - deltaTime;
        if (newLinear >= -0.79854f) {
            translateOX3 = glm::translate(glm::mat4(1.0), glm::vec3(0, newLinear, 0));
            linearOX3 = newLinear;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        float newLinear = linearOX3 + deltaTime;
        if (newLinear <= 0.12519f) {
            translateOX3 = glm::translate(glm::mat4(1.0), glm::vec3(0, newLinear, 0));
            linearOX3 = newLinear;
        }
    }
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: glfwInit failed\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Model Viewer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum ret = glewInit();
    if (GLEW_OK != ret) {
        fprintf(stderr, "ERROR: %s\n", glewGetErrorString(ret));
        return 1;
    }

    Shader shader("vertex_shader.glsl", "fragment_shader.glsl");
    shader.use();

    Model model("Model/model.obj");

    glm::vec3 lightPos(2.0f, 2.0f, 2.0f);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 modelMatrix = glm::mat4(1.0f);

        shader.setUniform("ourColor", 0.3f, 0.7f, 0.9f, 1.0f);
        shader.setUniformMat("view", view);
        shader.setUniformMat("projection", projection);
        shader.setUniformMat("model", modelMatrix);

        glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);
        shader.setVec3("lightColor", lightColor);

        glm::vec3 lightAmbient = lightColor * glm::vec3(0.2);
        glm::vec3 lightDiffuse = lightColor * glm::vec3(0.8);
        glm::vec3 lightSpecular = lightColor;

        shader.setVec3("light_1.ambient", lightAmbient);
        shader.setVec3("light_1.diffuse", lightDiffuse);
        shader.setVec3("light_1.specular", lightSpecular);
        shader.setVec3("light_1.position", lightPos);

        glm::vec3 matAmbient = glm::vec3(0.247, 0.199, 0.074);
        glm::vec3 matDiffuse = glm::vec3(0.751, 0.606, 0.226);
        glm::vec3 matSpecular = glm::vec3(0.628, 0.555, 0.366);
        float matShininess = 51.2f;

        shader.setVec3("mat_1.ambient", matAmbient);
        shader.setVec3("mat_1.diffuse", matDiffuse);
        shader.setVec3("mat_1.specular", matSpecular);
        shader.setFloat("mat_1.shininess", matShininess);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
        shader.setMat3("normalMatrix", normalMatrix);
        shader.setVec3("viewPos", cameraPos);

        model.Draw(shader, translateOX1, translateOX2, translateOX3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
