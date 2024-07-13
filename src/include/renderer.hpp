#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <map>
#include <shader.hpp>
#include <camera.hpp>
#include <model.hpp>
#include <pointLight.hpp>
#include <spotLight.hpp>
#include <gameObject.hpp>

enum RunStates
{
  NotRunning,
  DevRun,
  Build
};

class Renderer
{
public:
  std::unordered_map<std::string, Shader> Shaders;
  std::unordered_map<std::string, std::unique_ptr<GameObject>> GameObjects;
  std::unordered_map<std::string, std::unique_ptr<Model>> Models;
  std::unordered_map<std::string, PointLight> PointLights;
  std::unordered_map<std::string, SpotLight> SpotLights;
  GLFWwindow *window;

  std::vector<std::string> texturePaths;
  std::vector<std::string> scriptPaths;
  std::vector<std::string> filePaths;

  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  int ScreenW = 1900;
  int ScreenH = 1080;
  bool controllingCamera;

  float lastX = ScreenW / 2, lastY = ScreenH / 2;
  bool firstMouse = true;

  int activeCam = 0;
  Camera camera;
  Camera *gameCamera;

  bool isMinimized = false;

  Renderer(RunStates runGame = NotRunning);

  void addModel(std::string name, const std::string &path);

  void addModelDefault(const std::string &path);

  inline void setModelPosition(std::string name, glm::vec3 position)
  {
    Models.at(name)->position = position;
  }

  inline void setModelRotation(std::string name, glm::vec3 rotation)
  {
    Models.at(name)->rotation = rotation;
  }

  inline void setModelScale(std::string name, glm::vec3 scale)
  {
    Models.at(name)->scale = scale;
  }

  void addGameObject(std::string name, int type, const char *diffuseMap, const char *specularMap, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), std::string shaderName = "lightingShader");

  void addGameObjectDefault(int type, const char *diffuseMap, const char *specularMap, std::string shaderName = "lightingShader");

  void addPointLight(std::string name, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float intensity, std::string shaderName = "lightingShader")
  {
    PointLights.insert(std::make_pair(name, PointLight(position, ambient, diffuse, specular, constant, linear, quadratic, intensity, Shaders.at(shaderName), PointLights.size())));
  }

  void addPointLightDefault(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float intensity, std::string shaderName = "lightingShader");

  inline void setPointLightPosition(std::string name, glm::vec3 position, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setPosition(position, Shaders.at(shaderName));
  }

  inline void setPointLightDiffuse(std::string name, glm::vec3 diffuse, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setDiffuse(diffuse, Shaders.at(shaderName));
  }

  inline void setPointLightAmbient(std::string name, glm::vec3 ambient, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setAmbient(ambient, Shaders.at(shaderName));
  }

  inline void setPointLightSpecular(std::string name, glm::vec3 specular, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setSpecular(specular, Shaders.at(shaderName));
  }

  inline void setPointLightIntensity(std::string name, float intensity, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setIntensity(intensity, Shaders.at(shaderName));
  }

  inline void addShader(std::string name, Shader shader)
  {
    Shaders.insert(std::make_pair(name, shader));
  }

  void addSpotLight(std::string name, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, std::string shaderName = "lightingShader");

  inline void setSpotLightDiffuse(std::string name, glm::vec3 diffuse, std::string shaderName = "lightingShader")
  {
    SpotLights.at(name).setDiffuse(diffuse, Shaders.at(shaderName));
  }

  void addSpotLightDefault(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, std::string shaderName = "lightingShader");

  inline void setSpotLightPosition(std::string name, glm::vec3 position, std::string shaderName = "lightingShader")
  {
    SpotLights.at(name).setPosition(position, Shaders.at(shaderName));
  }

  inline void setSpotLightOn(std::string name, bool value)
  {
    SpotLights.at(name).setOnOrOff(value);
  }

  inline void setSpotLightDirection(std::string name, glm::vec3 direction, std::string shaderName = "lightingShader")
  {
    SpotLights.at(name).setDirection(direction, Shaders.at(shaderName));
  }

  inline void setDirectionLightDirection(glm::vec3 newDirection)
  {
    direction = newDirection;
  }

  inline void setDirectionLightAmbient(glm::vec3 newAmbient)
  {
    ambient = newAmbient;
  }
  inline void setDirectionLightDiffuse(glm::vec3 newDiffuse)
  {
    diffuse = newDiffuse;
  }
  inline void setDirectionLightSpecular(glm::vec3 newSpecular)
  {
    specular = newSpecular;
  }

  void draw(std::string lightingShaderName = "lightingShader", std::string lightObjShaderName = "lightObjShader");

  std::string copy_file_to_folder(const std::string &source, const std::string &destination_folder);

  void getTexturePaths();

  bool hasFileExtention(const std::string &file_path, const std::string &extension);
};

#endif