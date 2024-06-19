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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void processInput(GLFWwindow *window);
GLuint CreateFramebuffer(GLuint texture, int width, int height);
GLuint CreateOpenGLTexture(int width, int height);
void RenderToTexture(GLuint fbo, int width, int height);

Renderer renderer;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(renderer.window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  renderer.addModel("couch", "Objects/Couch/couch1.obj");
  renderer.addModel("backpack", "Objects/Backpack/backpack.obj");
  renderer.addModel("sphere", "Objects/Sphere/sphere.obj");

  renderer.setModelPosition("backpack", glm::vec3(20.0f, 0.0f, 0.0f));

  renderer.setModelPosition("couch", glm::vec3(0.0f, 0.0f, 20.0f));
  renderer.setModelScale("couch", glm::vec3(0.1f, 0.1f, 0.1f));

  renderer.setModelPosition("sphere", glm::vec3(-5.0f, 0.0f, 0.0f));

  renderer.addGameObject("cube1", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube1", glm::vec3(0.0f, 0.0f, 0.0f));

  renderer.addGameObject("cube2", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube2", glm::vec3(2.0f, 5.0f, -15.0f));

  renderer.addGameObject("cube3", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube3", glm::vec3(-1.5f, -2.2f, -2.5f));

  renderer.addGameObject("cube4", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube4", glm::vec3(-3.8f, -2.0f, -12.3f));

  renderer.addGameObject("cube5", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube5", glm::vec3(2.4f, -0.4f, -3.5f));

  renderer.addGameObject("cube6", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube6", glm::vec3(-1.7f, 3.0f, -7.5f));

  renderer.addGameObject("cube7", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube7", glm::vec3(1.3f, -2.0f, -2.5f));

  renderer.addGameObject("cube8", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube8", glm::vec3(1.5f, 2.0f, -2.5f));

  renderer.addGameObject("cube9", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube9", glm::vec3(1.5f, 0.2f, -1.5f));

  renderer.addGameObject("cube10", 0, "Textures/container2.png", "Textures/container2_specular.png");
  renderer.setGameObjectPosition("cube10", glm::vec3(-1.3f, 1.0f, -1.5f));

  renderer.addPointLight("pontLight1", glm::vec3(5.2f, 0.2f, 0.2f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 5.0f);
  renderer.addPointLight("pontLight2", glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

  renderer.addPointLight("pontLight3", glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

  renderer.addPointLight("pontLight4", glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

  renderer.addPointLight("pontLight5", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

  renderer.addPointLight("pontLight6", glm::vec3(-1.0f, 1.5f, 20.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3((cos(glfwGetTime()) + 1) / 2, (sin(glfwGetTime()) + 1) / 2, (cos(glfwGetTime()) + 1) / 2), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

  renderer.addSpotLight("spotLight1", renderer.camera.Position, renderer.camera.Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 12.5f, 17.5f);

  renderer.setSpotLightOn("spotLight1", false);

  renderer.setDirectionLightDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
  renderer.setDirectionLightAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
  renderer.setDirectionLightDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
  renderer.setDirectionLightSpecular(glm::vec3(0.5f, 0.5f, 0.5f));

  int textureWidth = 960;
  int textureHeight = 540;
  GLuint texture = CreateOpenGLTexture(textureWidth, textureHeight);
  GLuint fbo = CreateFramebuffer(texture, textureWidth, textureHeight);
  float aspectRatio = static_cast<float>(textureWidth) / static_cast<float>(textureHeight);
  while (!glfwWindowShouldClose(renderer.window))
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(renderer.window);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderer.setPointLightDiffuse("pontLight6", glm::vec3((cos(glfwGetTime()) + 1) / 2, (sin(glfwGetTime()) + 1) / 2, (cos(glfwGetTime()) + 1) / 2));

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(textureWidth, textureHeight), ImGuiCond_FirstUseEver);

    ImGui::Begin("OpenGL Render", NULL, ImGuiWindowFlags_NoCollapse);

    ImVec2 availSize = ImGui::GetContentRegionAvail();

    float imageWidth = availSize.x;
    float imageHeight = imageWidth / aspectRatio;

    if (imageHeight > availSize.y)
    {
      imageHeight = availSize.y;
      imageWidth = imageHeight * aspectRatio;
    }

    RenderToTexture(fbo, textureWidth, textureHeight);

    ImGui::Image(reinterpret_cast<ImTextureID>(texture), ImVec2(imageWidth, imageHeight), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

  if (renderer.controllingCamera)
  {
    float cameraSpeed = 20.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      renderer.camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      renderer.camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      renderer.camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      renderer.camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
      renderer.setSpotLightPosition("spotLight1", renderer.camera.Position);
      renderer.setSpotLightDirection("spotLight1", renderer.camera.Front);
    }
    else
    {
      renderer.setSpotLightPosition("spotLight1", glm::vec3(10000000.0f, 1000000000.5f, 200000000000.0f));
    }
  }
}

GLuint CreateOpenGLTexture(int width, int height)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}

GLuint CreateFramebuffer(GLuint texture, int width, int height)
{
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  GLuint depthBuffer;
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cerr << "Framebuffer is not complete!" << std::endl;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return fbo;
}

void RenderToTexture(GLuint fbo, int width, int height)
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, width, height);
  glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderer.draw();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
