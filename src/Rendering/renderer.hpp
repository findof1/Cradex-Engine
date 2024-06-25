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
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "Objects/pointLight.hpp"
#include "Objects/spotLight.hpp"
#include "Objects/gameObject.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

enum RunStates
{
  NotRunning,
  DevRun,
  Build
};

class Renderer
{
public:
  std::map<std::string, Shader> Shaders;
  std::map<std::string, GameObject *> GameObjects;
  std::map<std::string, Model *> Models;
  std::map<std::string, PointLight> PointLights;
  std::map<std::string, SpotLight> SpotLights;
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

  Camera camera;
  Camera *gameCamera;

  Renderer(RunStates runGame = NotRunning) : camera(glm::vec3(0.0f, 0.0f, 0.0f)), gameCamera(new Camera)
  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(ScreenW, ScreenH, "Cradex Engine", NULL, NULL);
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (runGame == NotRunning)
    {
      glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
      glfwSetCursorPosCallback(window, mouse_callback);
      glfwSetScrollCallback(window, scroll_callback);
      glfwSetMouseButtonCallback(window, mouseButtonCallback);
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      exit(EXIT_FAILURE);
    }

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
  void addModelDefault(const std::string &path)
  {
    if (!std::filesystem::exists(path))
    {
      return;
    }
    int i = 1;
    std::string name;

    while ((true))
    {
      if (GameObjects.find("model" + std::to_string(i)) == GameObjects.end())
      {
        name = "model" + std::to_string(i);
        break;
      }
      i++;
    }

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

  void addGameObject(std::string name, int type, const char *diffuseMap, const char *specularMap, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), std::string shaderName = "lightingShader")
  {
    GameObject *obj = new GameObject(type, diffuseMap, specularMap, Shaders.at(shaderName));
    obj->position = position;
    obj->rotation = rotation;
    obj->scale = scale;
    GameObjects.insert(std::make_pair(name, obj));
  }

  void addGameObjectDefault(int type, const char *diffuseMap, const char *specularMap, std::string shaderName = "lightingShader")
  {
    int i = 1;
    std::string name;
    std::string kind;
    if (type == 0)
    {
      kind = "cube";
    }

    while ((true))
    {
      if (GameObjects.find(kind + std::to_string(i)) == GameObjects.end())
      {
        name = kind + std::to_string(i);
        break;
      }
      i++;
    }

    GameObject *obj = new GameObject(type, diffuseMap, specularMap, Shaders.at(shaderName));
    GameObjects.insert(std::make_pair(name, obj));
  }

  void addPointLight(std::string name, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float intensity, std::string shaderName = "lightingShader")
  {
    PointLights.insert(std::make_pair(name, PointLight(position, ambient, diffuse, specular, constant, linear, quadratic, intensity, Shaders.at(shaderName), PointLights.size())));
  }

  void addPointLightDefault(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float intensity, std::string shaderName = "lightingShader")
  {
    int i = 1;
    std::string name;

    while ((true))
    {
      if (PointLights.find("pointLight" + std::to_string(i)) == PointLights.end())
      {
        name = "pointLight" + std::to_string(i);
        break;
      }
      i++;
    }

    PointLights.insert(std::make_pair(name, PointLight(position, ambient, diffuse, specular, constant, linear, quadratic, intensity, Shaders.at(shaderName), PointLights.size())));
  }

  void setPointLightPosition(std::string name, glm::vec3 position, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setPosition(position, Shaders.at(shaderName));
  }

  void setPointLightDiffuse(std::string name, glm::vec3 diffuse, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setDiffuse(diffuse, Shaders.at(shaderName));
  }

  void setPointLightAmbient(std::string name, glm::vec3 ambient, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setAmbient(ambient, Shaders.at(shaderName));
  }

  void setPointLightSpecular(std::string name, glm::vec3 specular, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setSpecular(specular, Shaders.at(shaderName));
  }

  void setPointLightIntensity(std::string name, float intensity, std::string shaderName = "lightingShader")
  {
    PointLights.at(name).setIntensity(intensity, Shaders.at(shaderName));
  }

  void addShader(std::string name, Shader shader)
  {
    Shaders.insert(std::make_pair(name, shader));
  }

  void addSpotLight(std::string name, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, std::string shaderName = "lightingShader")
  {
    SpotLights.insert(std::make_pair(name, SpotLight(position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff, Shaders.at(shaderName), SpotLights.size())));
  }

  void setSpotLightDiffuse(std::string name, glm::vec3 diffuse, std::string shaderName = "lightingShader")
  {
    SpotLights.at(name).setDiffuse(diffuse, Shaders.at(shaderName));
  }

  void addSpotLightDefault(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, std::string shaderName = "lightingShader")
  {
    int i = 1;
    std::string name;

    while ((true))
    {
      if (SpotLights.find("spotLight" + std::to_string(i)) == SpotLights.end())
      {
        name = "spotLight" + std::to_string(i);
        break;
      }
      i++;
    }

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

  std::string copy_file_to_folder(const std::string &source, const std::string &destination_folder)
  {
    try
    {
      std::filesystem::path source_path(source);
      std::filesystem::path destination_path(destination_folder);

      std::filesystem::create_directories(destination_path);

      destination_path /= source_path.filename();

      std::filesystem::copy_file(source_path, destination_path, std::filesystem::copy_options::overwrite_existing);

      return destination_path.string();
    }
    catch (const std::filesystem::filesystem_error &e)
    {
      std::cerr << "Filesystem error: " << e.what() << std::endl;
      return "";
    }
  }

  void getTexturePaths()
  {
    try
    {
      for (const auto &entry : std::filesystem::directory_iterator("Textures"))
      {
        std::string path = entry.path().u8string();

        if (hasFileExtention(path, ".png") || hasFileExtention(path, ".jpg") ||
            hasFileExtention(path, ".jpeg") || hasFileExtention(path, ".bmp") ||
            hasFileExtention(path, ".tga"))
        {
          texturePaths.push_back(path);
        }
        else if (hasFileExtention(path, ".lua"))
        {
          scriptPaths.push_back(path);
        }
        else
        {
          filePaths.push_back(path);
        }
      }
    }
    catch (std::filesystem::filesystem_error err)
    {
      std::cerr << "Error converting filepath to string: " << err.what() << std::endl;
    }
  }

  bool hasFileExtention(const std::string &file_path, const std::string &extension)
  {
    return file_path.size() >= extension.size() &&
           file_path.compare(file_path.size() - extension.size(), extension.size(), extension) == 0;
  }

private:
  static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
  {
    Renderer *renderer = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
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