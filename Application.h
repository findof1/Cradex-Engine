#ifndef APPLICATION_H
#define APPLICATION_H

#include "renderer.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "EngineUI.h"
#include <GLFW/glfw3.h>
#include "External/json.hpp"
#include <filesystem>

using json = nlohmann::json;

class Application
{
public:
  Renderer renderer;
  UI *engineUI;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  std::string scenePath;

  Application(const char *scenePath = "data.json") : scenePath(scenePath)
  {
    engineUI = new UI(&renderer);

    if (!std::filesystem::exists(scenePath))
    {
      initializeScene();
    }
    else
    {
      initializeScene();
    }
  }

  void run()
  {
    while (!glfwWindowShouldClose(renderer.window))
    {

      float currentFrame = static_cast<float>(glfwGetTime());
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(renderer.window);

      glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // run scrips here once I implement them
      renderer.setPointLightDiffuse("pontLight6", glm::vec3((cos(glfwGetTime()) + 1) / 2, (sin(glfwGetTime()) + 1) / 2, (cos(glfwGetTime()) + 1) / 2));

      engineUI->draw();

      glfwSwapBuffers(renderer.window);
      glfwPollEvents();
    }

    glfwTerminate();
  }

  void serialize()
  {
    json serializedJson;
    serializedJson["GameObjects"] = serializeGameObjects();
    serializedJson["PointLights"] = serializePointLights();
    serializedJson["SpotLights"] = serializeSpotLights();
    serializedJson["Models"] = serializeModels();

    std::ofstream outFile(scenePath, std::ios::out | std::ios::trunc);

    if (!outFile.is_open())
    {
      std::cerr << "Could not open or create the serialization file." << std::endl;
      exit(EXIT_FAILURE);
    }
    outFile << serializedJson.dump(4);

    outFile.close();
  }

private:
  json serializeGameObjects()
  {
    json objectArray = json::array();
    for (auto &object : renderer.GameObjects)
    {
      json serializedObject = object.second->serialize();
      serializedObject["name"] = object.first;
      objectArray.push_back(serializedObject);
    }

    return objectArray;
  }

  json serializePointLights()
  {
    json objectArray = json::array();
    for (auto &object : renderer.PointLights)
    {
      json serializedObject = object.second.serialize();
      serializedObject["name"] = object.first;
      objectArray.push_back(serializedObject);
    }

    return objectArray;
  }

  json serializeSpotLights()
  {
    json objectArray = json::array();
    for (auto &object : renderer.SpotLights)
    {
      json serializedObject = object.second.serialize();
      serializedObject["name"] = object.first;
      objectArray.push_back(serializedObject);
    }

    return objectArray;
  }

  json serializeModels()
  {
    json objectArray = json::array();
    for (auto &object : renderer.Models)
    {
      json serializedObject = object.second->serialize();
      serializedObject["name"] = object.first;
      objectArray.push_back(serializedObject);
    }

    return objectArray;
  }

  void processInput(GLFWwindow *window)
  {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (renderer.controllingCamera)
    {
      float cameraSpeed = 20.0f * deltaTime;

      if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed = 40.0f * deltaTime;

      if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraSpeed = 10.0f * deltaTime;

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
  void initializeScene()
  {

    renderer.addModel("couch", "Objects/Couch/couch1.obj");

    renderer.addModel("backpack", "Objects/Backpack/backpack.obj");

    renderer.addModel("sphere", "Objects/Sphere/sphere.obj");

    renderer.setModelPosition("backpack", glm::vec3(20.0f, 0.0f, 0.0f));

    renderer.setModelPosition("couch", glm::vec3(0.0f, 0.0f, 20.0f));
    renderer.setModelScale("couch", glm::vec3(0.1f, 0.1f, 0.1f));

    renderer.setModelPosition("sphere", glm::vec3(-5.0f, 0.0f, 0.0f));

    renderer.addGameObject("cube1", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube1")->position = glm::vec3(0.0f, 0.0f, 0.0f);

    renderer.addGameObject("cube2", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube2")->position = glm::vec3(2.0f, 5.0f, -15.0f);

    renderer.addGameObject("cube3", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube3")->position = glm::vec3(-1.5f, -2.2f, -2.5f);

    renderer.addGameObject("cube4", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube4")->position = glm::vec3(-3.8f, -2.0f, -12.3f);

    renderer.addGameObject("cube5", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube5")->position = glm::vec3(2.4f, -0.4f, -3.5f);

    renderer.addGameObject("cube6", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube6")->position = glm::vec3(-1.7f, 3.0f, -7.5f);

    renderer.addGameObject("cube7", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube7")->position = glm::vec3(1.3f, -2.0f, -2.5f);

    renderer.addGameObject("cube8", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube8")->position = glm::vec3(1.5f, 2.0f, -2.5f);

    renderer.addGameObject("cube9", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube9")->position = glm::vec3(1.5f, 0.2f, -1.5f);

    renderer.addGameObject("cube10", 0, "Textures/container2.png", "Textures/container2_specular.png");
    renderer.GameObjects.at("cube10")->position = glm::vec3(-1.3f, 1.0f, -1.5f);

    renderer.addPointLight("pontLight1", glm::vec3(5.2f, 0.2f, 0.2f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 5.0f);
    renderer.addPointLight("pontLight2", glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

    renderer.addPointLight("pontLight3", glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

    renderer.addPointLight("pontLight4", glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

    renderer.addPointLight("pontLight5", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

    renderer.addPointLight("pontLight6", glm::vec3(-1.0f, 1.5f, 20.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1.0f);

    renderer.addSpotLight("spotLight1", renderer.camera.Position, renderer.camera.Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 12.5f, 17.5f);

    renderer.setSpotLightOn("spotLight1", false);

    renderer.setDirectionLightDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
    renderer.setDirectionLightAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
    renderer.setDirectionLightDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
    renderer.setDirectionLightSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
  }
};

#endif