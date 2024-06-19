#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "pointLight.h"
#include "spotLight.h"
#include "gameObject.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Renderer
{
public:
  std::map<std::string, Shader> Shaders;
  std::map<std::string, GameObject *> GameObjects;
  std::map<std::string, Model *> Models;
  std::map<std::string, PointLight> PointLights;
  std::map<std::string, SpotLight> SpotLights;
  GLFWwindow *window;

  int ScreenW = 1920;
  int ScreenH = 1080;
  bool controllingCamera;

  float lastX = ScreenW / 2, lastY = ScreenH / 2;
  bool firstMouse = true;

  Camera camera;

  Renderer() : camera(glm::vec3(0.0f, 0.0f, 3.0f))
  {
    // GLFWframebuffersizefun framebuffer_size_callback, GLFWscrollfun scroll_callback, GLFWcursorposfun mouse_callback
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(ScreenW, ScreenH, "3D Renderer", NULL, NULL);
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      exit(EXIT_FAILURE);
    }
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);

    // normal
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // wireframe thingy
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShader("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");
    Shader lightCubeShader("Shaders/lightVertex.vs", "Shaders/lightFragment.fs");

    addShader("lightingShader", lightingShader);
    addShader("lightObjShader", lightCubeShader);

    direction = glm::vec3(0.0f);
    ambient = glm::vec3(0.0f);
    diffuse = glm::vec3(0.0f);
    specular = glm::vec3(0.0f);
  }

  void addModel(std::string name, const std::string &path)
  {
    Model *model = new Model(path);
    Models.insert(std::make_pair(name, model));
  }
  void setModelPosition(std::string name, glm::vec3 position)
  {
    Models.at(name)->position = position;
  }
  void setModelRotation(std::string name, glm::vec3 rotation)
  {
    Models.at(name)->rotation = rotation;
  }
  void setModelScale(std::string name, glm::vec3 scale)
  {
    Models.at(name)->scale = scale;
  }

  void addGameObject(std::string name, int type, const char *diffuseMap, const char *specularMap, std::string shaderName = "lightingShader")
  {
    GameObject *obj = new GameObject(type, diffuseMap, specularMap, Shaders.at(shaderName));
    GameObjects.insert(std::make_pair(name, obj));
  }

  void setGameObjectPosition(std::string name, glm::vec3 position)
  {
    GameObjects.at(name)->setPosition(position);
  }

  void setGameObjectRotation(std::string name, glm::vec3 rotation)
  {
    GameObjects.at(name)->setPosition(rotation);
  }

  void setGameObjectScale(std::string name, glm::vec3 scale)
  {
    GameObjects.at(name)->setPosition(scale);
  }

  void addPointLight(std::string name, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float intensity, std::string shaderName = "lightingShader")
  {
    PointLights.insert(std::make_pair(name, PointLight(position, ambient, diffuse, specular, constant, linear, quadratic, intensity, Shaders.at(shaderName), PointLights.size())));
  }

  void setPointLightDiffuse(std::string name, glm::vec3 diffuse, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setDiffuse(diffuse, Shaders.at(shaderName));
  }

  void addShader(std::string name, Shader shader)
  {
    Shaders.insert(std::make_pair(name, shader));
  }

  void addSpotLight(std::string name, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, std::string shaderName = "lightingShader")
  {
    SpotLights.insert(std::make_pair(name, SpotLight(position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff, Shaders.at(shaderName), SpotLights.size())));
  }

  void setSpotLightPosition(std::string name, glm::vec3 position, std::string shaderName = "lightingShader")
  {
    SpotLights.at(name).setPosition(position, Shaders.at(shaderName));
  }

  void setSpotLightOn(std::string name, bool value)
  {
    SpotLights.at(name).setOnOrOff(value);
  }

  void setSpotLightDirection(std::string name, glm::vec3 direction, std::string shaderName = "lightingShader")
  {
    SpotLights.at(name).setDirection(direction, Shaders.at(shaderName));
  }

  void setDirectionLightDirection(glm::vec3 newDirection)
  {
    direction = newDirection;
  }

  void setDirectionLightAmbient(glm::vec3 newAmbient)
  {
    ambient = newAmbient;
  }
  void setDirectionLightDiffuse(glm::vec3 newDiffuse)
  {
    diffuse = newDiffuse;
  }
  void setDirectionLightSpecular(glm::vec3 newSpecular)
  {
    specular = newSpecular;
  }

  void draw(std::string lightingShaderName = "lightingShader", std::string lightObjShaderName = "lightObjShader")
  {

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ScreenW / (float)ScreenH, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    Shader shader = Shaders.at(lightObjShaderName);
    Shader lightingShader = Shaders.at(lightingShaderName);
    shader.use();

    shader.setMat4("view", view);

    shader.setMat4("model", model);
    shader.setMat4("projection", projection);

    for (auto &light : PointLights)
    {
      light.second.draw(shader);
    }

    for (auto &light : SpotLights)
    {
      light.second.draw(shader);
    }

    lightingShader.use();

    lightingShader.setMat4("view", view);

    lightingShader.setMat4("projection", projection);
    lightingShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

    lightingShader.setVec3("dirLight.direction", direction.x, direction.y, direction.z);
    lightingShader.setVec3("dirLight.ambient", ambient.x, ambient.y, ambient.z);
    lightingShader.setVec3("dirLight.diffuse", diffuse.x, diffuse.y, diffuse.z);
    lightingShader.setVec3("dirLight.specular", specular.x, specular.y, specular.z);

    lightingShader.setInt("pointLightsCount", PointLights.size());

    lightingShader.setInt("spotLightsCount", SpotLights.size());

    lightingShader.setFloat("material.shininess", 32.0f);

    for (auto &object : GameObjects)
    {
      object.second->draw(lightingShader);
    }

    for (auto &model : Models)
    {
      model.second->draw(lightingShader);
    }
  }

private:
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
  {
    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    std::cout << "Hello World" << std::endl;

    if (renderer->controllingCamera)
    {
      glViewport(0, 0, width, height);
      renderer->ScreenW = width;
      renderer->ScreenH = height;
    }
  }

  static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
  {
    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    if (renderer->controllingCamera)
    {
      float xpos = static_cast<float>(xposIn);
      float ypos = static_cast<float>(yposIn);

      if (renderer->firstMouse)
      {
        renderer->lastX = xpos;
        renderer->lastY = ypos;
        renderer->firstMouse = false;
      }

      float xoffset = xpos - renderer->lastX;
      float yoffset = renderer->lastY - ypos;

      renderer->lastX = xpos;
      renderer->lastY = ypos;

      renderer->camera.ProcessMouseMovement(xoffset, yoffset);
    }
  }

  static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
  {
    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      renderer->controllingCamera = true;
    }
    else
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      renderer->controllingCamera = false;
      renderer->firstMouse = true;
    }
  }

  static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
  {
    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    if (renderer->controllingCamera)
    {
      renderer->camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
  }
};

#endif