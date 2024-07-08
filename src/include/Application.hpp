#ifndef APPLICATION_H
#define APPLICATION_H

#include <renderer.hpp>
#include <EngineUI.hpp>
#include <GLFW/glfw3.h>
#include <json.hpp>

using json = nlohmann::json;

class Application
{
public:
  Renderer renderer;
  UI *engineUI;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  RunStates runGameState;

  std::string scenePath;

  Application(RunStates runGameState = NotRunning, const char *scenePath = "data.json");
  void run();
  void runGame();
  void serialize();
  void binSerialize();
  int unserialize();

private:
  json serializeGameObjects();
  void binSerializeGameObjects(std::ofstream &ofs);

  json serializePointLights();
  void binSerializePointLights(std::ofstream &ofs);

  json serializeSpotLights();
  void binSerializeSpotLights(std::ofstream &ofs);

  json serializeModels();
  void binSerializeModels(std::ofstream &ofs);

  void processInput(GLFWwindow *window);
  void initializeScene();
};

#endif