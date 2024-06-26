#include <Application.hpp>
#include <renderer.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <EngineUI.hpp>
#include <GLFW/glfw3.h>
#include <json.hpp>
#include <filesystem>
#include <stdexcept>
#include <luaSupport.hpp>

Application::Application(RunStates runGameState, const char *scenePath) : scenePath(scenePath), renderer(runGameState), runGameState(runGameState)
{
  if (runGameState == NotRunning)
  {
    engineUI = new UI(&renderer);

    if (!std::filesystem::exists(scenePath))
    {
      initializeScene();
    }
    else
    {
      try
      {
        int res = unserialize();
        if (res == 1)
        {
          initializeScene();
        }
      }
      catch (const std::exception &err)
      {
        std::cout << err.what() << std::endl;
        initializeScene();
      }
    }
  }
}

void Application::run()
{
  while (!glfwWindowShouldClose(renderer.window))
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(renderer.window);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    engineUI->draw();

    glfwSwapBuffers(renderer.window);
    glfwPollEvents();
  }
  glfwTerminate();
}

void Application::runGame()
{
  unserialize();

  LuaHandler luaHandler(&renderer);

  luaHandler.registerLuaFunctions();

  std::vector<std::string> scriptPaths;

  for (const auto &entry : std::filesystem::directory_iterator("Scripts"))
  {
    std::string path = entry.path().u8string();
    scriptPaths.push_back(path);
  }

  for (const auto path : scriptPaths)
  {
    if (!luaHandler.loadScript(path))
    {
      std::cerr << "Failed to load script: " << path << std::endl;
      exit(EXIT_FAILURE);
    }

    luaHandler.executeFunction("start");
  }
  glViewport(0, 0, renderer.ScreenW, renderer.ScreenH);

  while (!glfwWindowShouldClose(renderer.window))
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(renderer.window, GLFW_KEY_E) == GLFW_PRESS)
    {
      glfwTerminate();
      std::system("start main.exe >nul 2>&1");
      exit(EXIT_FAILURE);
    }

    renderer.camera = *renderer.gameCamera;

    luaHandler.setGlobalNumber("deltaTime", deltaTime);

    for (const auto path : scriptPaths)
    {
      if (!luaHandler.loadScript(path))
      {
        std::cerr << "Failed to load script: " << path << std::endl;
        exit(EXIT_FAILURE);
      }

      luaHandler.executeFunction("update");
    }

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.draw();

    glfwSwapBuffers(renderer.window);
    glfwPollEvents();
  }
}

void Application::serialize()
{
  json serializedJson;
  serializedJson["GameObjects"] = serializeGameObjects();
  serializedJson["PointLights"] = serializePointLights();
  serializedJson["SpotLights"] = serializeSpotLights();
  serializedJson["Models"] = serializeModels();

  json serializedDirectionalLight;

  json serializedDirection;

  serializedDirection["x"] = renderer.direction.x;
  serializedDirection["y"] = renderer.direction.y;
  serializedDirection["z"] = renderer.direction.z;

  json serializedAmbient;
  serializedAmbient["r"] = renderer.ambient.x;
  serializedAmbient["g"] = renderer.ambient.y;
  serializedAmbient["b"] = renderer.ambient.z;

  json serializedDiffuse;
  serializedDiffuse["r"] = renderer.diffuse.x;
  serializedDiffuse["g"] = renderer.diffuse.y;
  serializedDiffuse["b"] = renderer.diffuse.z;

  json serializedSpecular;
  serializedSpecular["r"] = renderer.specular.x;
  serializedSpecular["g"] = renderer.specular.y;
  serializedSpecular["b"] = renderer.specular.z;

  serializedDirectionalLight["direction"] = serializedDirection;
  serializedDirectionalLight["ambient"] = serializedAmbient;
  serializedDirectionalLight["diffuse"] = serializedDiffuse;
  serializedDirectionalLight["specular"] = serializedSpecular;

  serializedJson["DirectionalLight"] = serializedDirectionalLight;

  json serializedCamera;

  json serializedPosition;
  serializedPosition["x"] = renderer.gameCamera->Position.x;
  serializedPosition["y"] = renderer.gameCamera->Position.y;
  serializedPosition["z"] = renderer.gameCamera->Position.z;

  serializedCamera["yaw"] = renderer.gameCamera->Yaw;
  serializedCamera["pitch"] = renderer.gameCamera->Pitch;
  serializedCamera["position"] = serializedPosition;

  serializedJson["camera"] = serializedCamera;

  std::ofstream outFile(scenePath, std::ios::out | std::ios::trunc);

  if (!outFile.is_open())
  {
    std::cerr << "Could not open or create the serialization file." << std::endl;
    exit(EXIT_FAILURE);
  }
  outFile << serializedJson.dump(4);

  outFile.close();
}

int Application::unserialize()
{
  std::ifstream inputFile(scenePath);
  if (!inputFile.is_open())
  {
    std::cerr << "Failed to open file." << std::endl;
    return 1;
  }

  std::string jsonString((std::istreambuf_iterator<char>(inputFile)),
                         std::istreambuf_iterator<char>());

  inputFile.close();

  json data = json::parse(jsonString);

  auto gameObjects = data["GameObjects"];
  for (auto &gameObject : gameObjects)
  {
    std::string name = gameObject["name"];
    float posX = gameObject["position"]["x"];
    float posY = gameObject["position"]["y"];
    float posZ = gameObject["position"]["z"];

    float rotationX = gameObject["rotation"]["x"];
    float rotationY = gameObject["rotation"]["y"];
    float rotationZ = gameObject["rotation"]["z"];

    float scaleX = gameObject["scale"]["x"];
    float scaleY = gameObject["scale"]["y"];
    float scaleZ = gameObject["scale"]["z"];

    int type;

    if (gameObject.contains("type") && !gameObject["type"].is_null())
    {

      type = gameObject["type"];
    }
    else
    {

      type = 0;
    }

    std::string diffusePath = gameObject["diffusePath"];
    std::string specularPath = gameObject["specularPath"];
    renderer.addGameObject(name, type, diffusePath.c_str(), specularPath.c_str(), glm::vec3(posX, posY, posZ), glm::vec3(rotationX, rotationY, rotationZ), glm::vec3(scaleX, scaleY, scaleZ));
  }

  auto pointLights = data["PointLights"];
  for (auto &pointLight : pointLights)
  {
    std::string name = pointLight["name"];
    float posX = pointLight["position"]["x"];
    float posY = pointLight["position"]["y"];
    float posZ = pointLight["position"]["z"];

    float ambientR = pointLight["ambient"]["r"];
    float ambientG = pointLight["ambient"]["g"];
    float ambientB = pointLight["ambient"]["b"];

    float diffuseR = pointLight["diffuse"]["r"];
    float diffuseG = pointLight["diffuse"]["g"];
    float diffuseB = pointLight["diffuse"]["b"];

    float specularR = pointLight["specular"]["r"];
    float specularG = pointLight["specular"]["g"];
    float specularB = pointLight["specular"]["b"];

    float constant = pointLight["constant"];
    float linear = pointLight["linear"];
    float quadratic = pointLight["quadratic"];

    float intensity = pointLight["intensity"];

    renderer.addPointLight(name, glm::vec3(posX, posY, posZ), glm::vec3(ambientR, ambientG, ambientB), glm::vec3(diffuseR, diffuseG, diffuseB), glm::vec3(specularR, specularG, specularB), constant, linear, quadratic, intensity);
  }

  auto spotLights = data["SpotLights"];
  for (auto &spotLight : spotLights)
  {
    std::string name = spotLight["name"];
    float posX = spotLight["position"]["x"];
    float posY = spotLight["position"]["y"];
    float posZ = spotLight["position"]["z"];

    float directionX = spotLight["direction"]["x"];
    float directionY = spotLight["direction"]["y"];
    float directionZ = spotLight["direction"]["z"];

    float ambientR = spotLight["ambient"]["r"];
    float ambientG = spotLight["ambient"]["g"];
    float ambientB = spotLight["ambient"]["b"];

    float diffuseR = spotLight["diffuse"]["r"];
    float diffuseG = spotLight["diffuse"]["g"];
    float diffuseB = spotLight["diffuse"]["b"];

    float specularR = spotLight["specular"]["r"];
    float specularG = spotLight["specular"]["g"];
    float specularB = spotLight["specular"]["b"];

    float constant = spotLight["constant"];
    float linear = spotLight["linear"];
    float quadratic = spotLight["quadratic"];

    float cutOff = spotLight["cutOff"];
    float outerCutOff = spotLight["outerCutOff"];

    bool on = spotLight["on"];

    renderer.addSpotLight(name, glm::vec3(posX, posY, posZ), glm::vec3(directionX, directionY, directionZ), glm::vec3(ambientR, ambientG, ambientB), glm::vec3(diffuseR, diffuseG, diffuseB), glm::vec3(specularR, specularG, specularB), constant, linear, quadratic, cutOff, outerCutOff);

    renderer.SpotLights.at(name).on = on;
  }

  auto Models = data["Models"];
  for (auto &model : Models)
  {
    std::string name = model["name"];
    float posX = model["position"]["x"];
    float posY = model["position"]["y"];
    float posZ = model["position"]["z"];

    float rotationX = model["rotation"]["x"];
    float rotationY = model["rotation"]["y"];
    float rotationZ = model["rotation"]["z"];

    float scaleX = model["scale"]["x"];
    float scaleY = model["scale"]["y"];
    float scaleZ = model["scale"]["z"];

    std::string path = model["path"];

    renderer.addModel(name, path);
    renderer.setModelPosition(name, glm::vec3(posX, posY, posZ));
    renderer.setModelRotation(name, glm::vec3(rotationX, rotationY, rotationZ));
    renderer.setModelScale(name, glm::vec3(scaleX, scaleY, scaleZ));
  }

  auto DirectionalLight = data["DirectionalLight"];

  renderer.setDirectionLightDirection(glm::vec3(DirectionalLight["direction"]["x"], DirectionalLight["direction"]["y"], DirectionalLight["direction"]["z"]));
  renderer.setDirectionLightAmbient(glm::vec3(DirectionalLight["ambient"]["r"], DirectionalLight["ambient"]["g"], DirectionalLight["ambient"]["b"]));
  renderer.setDirectionLightDiffuse(glm::vec3(DirectionalLight["diffuse"]["r"], DirectionalLight["diffuse"]["g"], DirectionalLight["diffuse"]["b"]));
  renderer.setDirectionLightSpecular(glm::vec3(DirectionalLight["specular"]["r"], DirectionalLight["specular"]["g"], DirectionalLight["specular"]["b"]));

  auto gameCam = data["camera"];

  renderer.gameCamera = new Camera(glm::vec3(gameCam["position"]["x"], gameCam["position"]["y"], gameCam["position"]["z"]), gameCam["yaw"], gameCam["pitch"]);

  return 0;
}

json Application::serializeGameObjects()
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

json Application::serializePointLights()
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

json Application::serializeSpotLights()
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

json Application::serializeModels()
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

void Application::processInput(GLFWwindow *window)
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

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
      serialize();
      glfwTerminate();
      renderer = Renderer(DevRun);

      runGame();
    }
  }
}

void Application::initializeScene()
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