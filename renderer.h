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

class Renderer
{
public:
  std::map<std::string, Shader> Shaders;
  std::map<std::string, GameObject> GameObjects;
  std::map<std::string, PointLight> PointLights;
  std::map<std::string, SpotLight> SpotLights;
  GLFWwindow *window;

  int ScreenW = 1920;
  int ScreenH = 1080;

  Camera camera;

  Renderer(GLFWframebuffersizefun framebuffer_size_callback, GLFWscrollfun scroll_callback, GLFWcursorposfun mouse_callback) : camera(glm::vec3(0.0f, 0.0f, 3.0f))
  {

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      exit(EXIT_FAILURE);
    }
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);

    // normal
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // wireframe thingy
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  ~Renderer()
  {
  }

  void addGameObject(std::string name, int type, const char *diffuseMap, const char *specularMap, std::string shaderName)
  {
    GameObjects.insert(std::make_pair(name, GameObject(type, diffuseMap, specularMap, Shaders.at(shaderName))));
  }

  void setGameObjectPosition(std::string name, glm::vec3 position)
  {
    GameObjects.at(name).setPosition(position);
  }

  void setGameObjectRotation(std::string name, glm::vec3 rotation)
  {
    GameObjects.at(name).setPosition(rotation);
  }

  void setGameObjectScale(std::string name, glm::vec3 scale)
  {
    GameObjects.at(name).setPosition(scale);
  }

  void addPointLight(std::string name, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float intensity, std::string shaderName)
  {
    PointLights.insert(std::make_pair(name, PointLight(position, ambient, diffuse, specular, constant, linear, quadratic, intensity, Shaders.at(shaderName), PointLights.size())));
  }

  void addShader(std::string name, Shader shader)
  {
    Shaders.insert(std::make_pair(name, shader));
  }

  void addSpotLight(std::string name, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, std::string shaderName)
  {
    SpotLights.insert(std::make_pair(name, SpotLight(position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff, Shaders.at(shaderName), SpotLights.size())));
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
  void draw(std::string lightingShaderName, std::string lightObjShaderName)
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
      object.second.draw(lightingShader);
    }
  }

private:
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

#endif