#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "External/stb_image.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "pointLight.h"
#include "spotLight.h"
#include "renderer.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);
void processInput(GLFWwindow *window);

int ScreenW = 1920;
int ScreenH = 1080;

Renderer renderer(framebuffer_size_callback, scroll_callback, mouse_callback);

float lastX = ScreenW / 2, lastY = ScreenH / 2;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
  Shader lightingShader("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");
  Shader lightCubeShader("Shaders/lightVertex.vs", "Shaders/lightFragment.fs");

  renderer.addShader("lightingShader", lightingShader);
  renderer.addShader("lightObjShader", lightCubeShader);

  renderer.addGameObject("cube1", 0, "Textures/container2.png", "Textures/container2_specular.png", "lightingShader");

  renderer.addPointLight("pontLight1", glm::vec3(5.2f, 0.2f, 0.2f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 5.0f, "lightingShader");
  renderer.addPointLight("pontLight2", glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f, "lightingShader");

  renderer.addPointLight("pontLight3", glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f, "lightingShader");

  renderer.addPointLight("pontLight4", glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f, "lightingShader");

  renderer.addPointLight("pontLight5", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f, "lightingShader");

  renderer.addPointLight("pontLight6", glm::vec3(-1.0f, 1.5f, 20.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3((cos(glfwGetTime()) + 1) / 2, (sin(glfwGetTime()) + 1) / 2, (cos(glfwGetTime()) + 1) / 2), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f, "lightingShader");

  renderer.addSpotLight("spotLight1", renderer.camera.Position, renderer.camera.Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 12.5f, 17.5f, "lightingShader");

  renderer.setDirectionLightDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
  renderer.setDirectionLightAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
  renderer.setDirectionLightDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
  renderer.setDirectionLightSpecular(glm::vec3(0.5f, 0.5f, 0.5f));

  while (!glfwWindowShouldClose(renderer.window))
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(renderer.window);

    glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.draw("lightingShader", "lightObjShader");

    glfwSwapBuffers(renderer.window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = 20.0f * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    renderer.camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    renderer.camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    renderer.camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    renderer.camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
  ScreenW = width;
  ScreenH = height;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
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
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  renderer.camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  renderer.camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(const char *path)
{
  unsigned int textureID;
  glGenTextures(1, &textureID);

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

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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