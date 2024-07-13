#include <luaSupport.hpp>

static int lua_addGameObject(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  int type = luaL_checkinteger(L, 2);
  const char *diffuseMap = luaL_checkstring(L, 3);
  const char *specularMap = luaL_checkstring(L, 4);
  glm::vec3 position(luaL_checknumber(L, 5), luaL_checknumber(L, 6), luaL_checknumber(L, 7));
  glm::vec3 rotation(luaL_checknumber(L, 8), luaL_checknumber(L, 9), luaL_checknumber(L, 10));
  glm::vec3 scale(luaL_checknumber(L, 11), luaL_checknumber(L, 12), luaL_checknumber(L, 13));

  renderer->addGameObject(name, type, diffuseMap, specularMap, position, rotation, scale);
  return 0;
}

static int lua_deleteGameObject(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);

  if (renderer->GameObjects.find(name) != renderer->GameObjects.end())
    renderer->GameObjects.erase(name);
  return 0;
}

static int lua_setGameObjectPosition(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 position(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->GameObjects.at(name)->position = position;
  return 0;
}

static int lua_setGameObjectRotation(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 rotation(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->GameObjects.at(name)->rotation = rotation;
  return 0;
}

static int lua_setGameObjectScale(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 scale(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->GameObjects.at(name)->scale = scale;
  return 0;
}

static int lua_addModel(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  const char *path = luaL_checkstring(L, 2);

  renderer->addModel(name, path);
  return 0;
}

static int lua_deleteModel(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);

  if (renderer->Models.find(name) != renderer->Models.end())
    renderer->Models.erase(name);
  return 0;
}

static int lua_setModelPosition(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 position(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->setModelPosition(name, position);
  return 0;
}

static int lua_setModelRotation(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 rotation(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->setModelRotation(name, rotation);
  return 0;
}

static int lua_setModelScale(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 scale(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->setModelScale(name, scale);
  return 0;
}

static int lua_addPointLight(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 position(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  glm::vec3 ambient(luaL_checknumber(L, 5), luaL_checknumber(L, 6), luaL_checknumber(L, 7));
  glm::vec3 diffuse(luaL_checknumber(L, 8), luaL_checknumber(L, 9), luaL_checknumber(L, 10));
  glm::vec3 specular(luaL_checknumber(L, 11), luaL_checknumber(L, 12), luaL_checknumber(L, 13));
  float constant = luaL_checknumber(L, 14);
  float linear = luaL_checknumber(L, 15);
  float quadratic = luaL_checknumber(L, 16);
  float intensity = luaL_checknumber(L, 17);

  renderer->addPointLight(name, position, ambient, diffuse, specular, constant, linear, quadratic, intensity);
  return 0;
}

static int lua_deletePointLight(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);

  if (renderer->PointLights.find(name) != renderer->PointLights.end())
    renderer->PointLights.erase(name);
  return 0;
}

static int lua_setPointLightPosition(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 position(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->PointLights.at(name).position = position;
  return 0;
}

static int lua_setPointLightDiffuse(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 diffuse(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->PointLights.at(name).diffuse = diffuse;
  return 0;
}

static int lua_setPointLightAmbient(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 ambient(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->PointLights.at(name).ambient = ambient;
  return 0;
}

static int lua_setPointLightSpecular(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 specular(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->PointLights.at(name).specular = specular;
  return 0;
}

static int lua_setPointLightOptions(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  float constant = luaL_checknumber(L, 2);
  float linear = luaL_checknumber(L, 3);
  float quadratic = luaL_checknumber(L, 4);

  renderer->PointLights.at(name).constant = constant;
  renderer->PointLights.at(name).linear = linear;
  renderer->PointLights.at(name).quadratic = quadratic;
  return 0;
}

static int lua_setPointLightIntensity(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  float intensity = luaL_checknumber(L, 2);

  renderer->PointLights.at(name).intensity = intensity;

  return 0;
}

static int lua_addSpotLight(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 position(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
  glm::vec3 direction(luaL_checknumber(L, 5), luaL_checknumber(L, 6), luaL_checknumber(L, 7));
  glm::vec3 ambient(luaL_checknumber(L, 8), luaL_checknumber(L, 9), luaL_checknumber(L, 10));
  glm::vec3 diffuse(luaL_checknumber(L, 11), luaL_checknumber(L, 12), luaL_checknumber(L, 13));
  glm::vec3 specular(luaL_checknumber(L, 14), luaL_checknumber(L, 15), luaL_checknumber(L, 16));
  float constant = luaL_checknumber(L, 17);
  float linear = luaL_checknumber(L, 18);
  float quadratic = luaL_checknumber(L, 19);
  float cutOff = luaL_checknumber(L, 20);
  float outerCutOff = luaL_checknumber(L, 21);

  renderer->addSpotLight(name, position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff);
  return 0;
}

static int lua_deleteSpotLight(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);

  if (renderer->SpotLights.find(name) != renderer->SpotLights.end())
    renderer->SpotLights.erase(name);
  return 0;
}

static int lua_setSpotLightPosition(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 position(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->SpotLights.at(name).position = position;
  return 0;
}

static int lua_setSpotLightDiffuse(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 diffuse(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->SpotLights.at(name).diffuse = diffuse;
  return 0;
}

static int lua_setSpotLightAmbient(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 ambient(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->SpotLights.at(name).ambient = ambient;
  return 0;
}

static int lua_setSpotLightSpecular(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  glm::vec3 specular(luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));

  renderer->SpotLights.at(name).specular = specular;
  return 0;
}

static int lua_setSpotLightOptions(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  float constant = luaL_checknumber(L, 2);
  float linear = luaL_checknumber(L, 3);
  float quadratic = luaL_checknumber(L, 4);

  renderer->SpotLights.at(name).constant = constant;
  renderer->SpotLights.at(name).linear = linear;
  renderer->SpotLights.at(name).quadratic = quadratic;
  return 0;
}

static int lua_setSpotLightCutoff(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  float cutOff = luaL_checknumber(L, 2);
  float outerCutOff = luaL_checknumber(L, 3);

  renderer->SpotLights.at(name).cutOff = cutOff;
  renderer->SpotLights.at(name).outerCutOff = outerCutOff;

  return 0;
}

static int lua_setSpotLightOn(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *name = luaL_checkstring(L, 1);
  bool on = lua_toboolean(L, 2);

  renderer->SpotLights.at(name).on = on;

  return 0;
}

int stringToGlfwKey(const char *key)
{
  if (strcmp(key, "A") == 0)
    return GLFW_KEY_A;
  if (strcmp(key, "B") == 0)
    return GLFW_KEY_B;
  if (strcmp(key, "C") == 0)
    return GLFW_KEY_C;
  if (strcmp(key, "D") == 0)
    return GLFW_KEY_D;
  if (strcmp(key, "E") == 0)
    return GLFW_KEY_E;
  if (strcmp(key, "F") == 0)
    return GLFW_KEY_F;
  if (strcmp(key, "G") == 0)
    return GLFW_KEY_G;
  if (strcmp(key, "H") == 0)
    return GLFW_KEY_H;
  if (strcmp(key, "I") == 0)
    return GLFW_KEY_I;
  if (strcmp(key, "J") == 0)
    return GLFW_KEY_J;
  if (strcmp(key, "K") == 0)
    return GLFW_KEY_K;
  if (strcmp(key, "L") == 0)
    return GLFW_KEY_L;
  if (strcmp(key, "M") == 0)
    return GLFW_KEY_M;
  if (strcmp(key, "N") == 0)
    return GLFW_KEY_N;
  if (strcmp(key, "O") == 0)
    return GLFW_KEY_O;
  if (strcmp(key, "P") == 0)
    return GLFW_KEY_P;
  if (strcmp(key, "Q") == 0)
    return GLFW_KEY_Q;
  if (strcmp(key, "R") == 0)
    return GLFW_KEY_R;
  if (strcmp(key, "S") == 0)
    return GLFW_KEY_S;
  if (strcmp(key, "T") == 0)
    return GLFW_KEY_T;
  if (strcmp(key, "U") == 0)
    return GLFW_KEY_U;
  if (strcmp(key, "V") == 0)
    return GLFW_KEY_V;
  if (strcmp(key, "W") == 0)
    return GLFW_KEY_W;
  if (strcmp(key, "X") == 0)
    return GLFW_KEY_X;
  if (strcmp(key, "Y") == 0)
    return GLFW_KEY_Y;
  if (strcmp(key, "Z") == 0)
    return GLFW_KEY_Z;
  if (strcmp(key, "1") == 0)
    return GLFW_KEY_1;
  if (strcmp(key, "2") == 0)
    return GLFW_KEY_2;
  if (strcmp(key, "3") == 0)
    return GLFW_KEY_3;
  if (strcmp(key, "4") == 0)
    return GLFW_KEY_4;
  if (strcmp(key, "5") == 0)
    return GLFW_KEY_5;
  if (strcmp(key, "6") == 0)
    return GLFW_KEY_6;
  if (strcmp(key, "7") == 0)
    return GLFW_KEY_7;
  if (strcmp(key, "8") == 0)
    return GLFW_KEY_8;
  if (strcmp(key, "9") == 0)
    return GLFW_KEY_9;
  if (strcmp(key, "0") == 0)
    return GLFW_KEY_0;
  if (strcmp(key, "SPACE") == 0)
    return GLFW_KEY_SPACE;
  if (strcmp(key, "ENTER") == 0)
    return GLFW_KEY_ENTER;
  if (strcmp(key, "ESCAPE") == 0)
    return GLFW_KEY_ESCAPE;
  if (strcmp(key, "LEFT_SHIFT") == 0)
    return GLFW_KEY_LEFT_SHIFT;
  if (strcmp(key, "RIGHT_SHIFT") == 0)
    return GLFW_KEY_RIGHT_SHIFT;
  if (strcmp(key, "LEFT_CONTROL") == 0)
    return GLFW_KEY_LEFT_CONTROL;
  if (strcmp(key, "RIGHT_CONTROL") == 0)
    return GLFW_KEY_RIGHT_CONTROL;
  if (strcmp(key, "LEFT_ALT") == 0)
    return GLFW_KEY_LEFT_ALT;
  if (strcmp(key, "RIGHT_ALT") == 0)
    return GLFW_KEY_RIGHT_ALT;
  if (strcmp(key, "TAB") == 0)
    return GLFW_KEY_TAB;
  if (strcmp(key, "BACKSPACE") == 0)
    return GLFW_KEY_BACKSPACE;
  if (strcmp(key, "INSERT") == 0)
    return GLFW_KEY_INSERT;
  if (strcmp(key, "DELETE") == 0)
    return GLFW_KEY_DELETE;
  if (strcmp(key, "HOME") == 0)
    return GLFW_KEY_HOME;
  if (strcmp(key, "END") == 0)
    return GLFW_KEY_END;
  if (strcmp(key, "PAGE_UP") == 0)
    return GLFW_KEY_PAGE_UP;
  if (strcmp(key, "PAGE_DOWN") == 0)
    return GLFW_KEY_PAGE_DOWN;
  if (strcmp(key, "LEFT") == 0)
    return GLFW_KEY_LEFT;
  if (strcmp(key, "RIGHT") == 0)
    return GLFW_KEY_RIGHT;
  if (strcmp(key, "UP") == 0)
    return GLFW_KEY_UP;
  if (strcmp(key, "DOWN") == 0)
    return GLFW_KEY_DOWN;
  if (strcmp(key, "F1") == 0)
    return GLFW_KEY_F1;
  if (strcmp(key, "F2") == 0)
    return GLFW_KEY_F2;
  if (strcmp(key, "F3") == 0)
    return GLFW_KEY_F3;
  if (strcmp(key, "F4") == 0)
    return GLFW_KEY_F4;
  if (strcmp(key, "F5") == 0)
    return GLFW_KEY_F5;
  if (strcmp(key, "F6") == 0)
    return GLFW_KEY_F6;
  if (strcmp(key, "F7") == 0)
    return GLFW_KEY_F7;
  if (strcmp(key, "F8") == 0)
    return GLFW_KEY_F8;
  if (strcmp(key, "F9") == 0)
    return GLFW_KEY_F9;
  if (strcmp(key, "F10") == 0)
    return GLFW_KEY_F10;
  if (strcmp(key, "F11") == 0)
    return GLFW_KEY_F11;
  if (strcmp(key, "F12") == 0)
    return GLFW_KEY_F12;

  return GLFW_KEY_UNKNOWN;
}

static int lua_isKeyPressed(lua_State *L)
{
  Renderer *renderer = (Renderer *)lua_touserdata(L, lua_upvalueindex(1));
  const char *key = luaL_checkstring(L, 1);
  if (glfwGetKey(renderer->window, stringToGlfwKey(key)) == GLFW_PRESS)
  {
    lua_pushboolean(L, 1);
  }
  else
  {
    lua_pushboolean(L, 0);
  }
  return 1;
}

LuaHandler::LuaHandler(Renderer *renderer) : renderer(renderer)
{
  L = luaL_newstate();
  luaL_openlibs(L);
}

LuaHandler::~LuaHandler()
{
  lua_close(L);
}

bool LuaHandler::loadScript(const std::string &scriptPath)
{
  if (luaL_loadfile(L, scriptPath.c_str()) || lua_pcall(L, 0, 0, 0))
  {
    std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
    return false;
  }
  return true;
}

bool LuaHandler::executeFunction(const std::string &functionName)
{
  lua_getglobal(L, functionName.c_str());
  if (!lua_isfunction(L, -1))
  {
    std::cerr << "Error: function " << functionName << " not found" << std::endl;
    return false;
  }

  if (lua_pcall(L, 0, 0, 0) != 0)
  {
    std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
    return false;
  }

  return true;
}

int LuaHandler::getGlobalInt(const std::string &globalName)
{
  lua_getglobal(L, globalName.c_str());
  if (!lua_isnumber(L, -1))
  {
    std::cerr << "Error: global " << globalName << " is not a number" << std::endl;
    return 0;
  }
  int value = (int)lua_tonumber(L, -1);
  lua_pop(L, 1);
  return value;
}

void LuaHandler::setGlobalInt(const std::string &globalName, int value)
{
  lua_pushnumber(L, value);
  lua_setglobal(L, globalName.c_str());
}

void LuaHandler::setGlobalNumber(const std::string &globalName, int value)
{
  lua_pushnumber(L, value);
  lua_setglobal(L, globalName.c_str());
}

void LuaHandler::registerLuaFunctions()
{
  lua_pushlightuserdata(L, renderer);

  lua_pushcclosure(L, lua_addGameObject, 1);
  lua_setglobal(L, "addGameObject");

  lua_pushcclosure(L, lua_deleteGameObject, 1);
  lua_setglobal(L, "deleteGameObject");

  lua_pushcclosure(L, lua_setGameObjectPosition, 1);
  lua_setglobal(L, "setGameObjectPosition");

  lua_pushcclosure(L, lua_setGameObjectRotation, 1);
  lua_setglobal(L, "setGameObjectRotation");

  lua_pushcclosure(L, lua_setGameObjectScale, 1);
  lua_setglobal(L, "setGameObjectScale");

  lua_pushcclosure(L, lua_addModel, 1);
  lua_setglobal(L, "addModel");

  lua_pushcclosure(L, lua_deleteModel, 1);
  lua_setglobal(L, "deleteModel");

  lua_pushcclosure(L, lua_setModelPosition, 1);
  lua_setglobal(L, "setModelPosition");

  lua_pushcclosure(L, lua_setModelRotation, 1);
  lua_setglobal(L, "setModelRotation");

  lua_pushcclosure(L, lua_setModelScale, 1);
  lua_setglobal(L, "setModelScale");

  lua_pushcclosure(L, lua_addPointLight, 1);
  lua_setglobal(L, "addPointLight");

  lua_pushcclosure(L, lua_deletePointLight, 1);
  lua_setglobal(L, "deletePointLight");

  lua_pushcclosure(L, lua_setPointLightPosition, 1);
  lua_setglobal(L, "setPointLightPosition");

  lua_pushcclosure(L, lua_setPointLightDiffuse, 1);
  lua_setglobal(L, "setPointLightDiffuse");

  lua_pushcclosure(L, lua_setPointLightAmbient, 1);
  lua_setglobal(L, "setPointLightAmbient");

  lua_pushcclosure(L, lua_setPointLightSpecular, 1);
  lua_setglobal(L, "setPointLightSpecular");

  lua_pushcclosure(L, lua_setPointLightOptions, 1);
  lua_setglobal(L, "setPointLightOptions");

  lua_pushcclosure(L, lua_setPointLightIntensity, 1);
  lua_setglobal(L, "setPointLightIntensity");

  lua_pushcclosure(L, lua_addSpotLight, 1);
  lua_setglobal(L, "addSpotLight");

  lua_pushcclosure(L, lua_deleteSpotLight, 1);
  lua_setglobal(L, "deleteSpotLight");

  lua_pushcclosure(L, lua_setSpotLightPosition, 1);
  lua_setglobal(L, "setSpotLightPosition");

  lua_pushcclosure(L, lua_setSpotLightDiffuse, 1);
  lua_setglobal(L, "setSpotLightDiffuse");

  lua_pushcclosure(L, lua_setSpotLightAmbient, 1);
  lua_setglobal(L, "setSpotLightAmbient");

  lua_pushcclosure(L, lua_setSpotLightSpecular, 1);
  lua_setglobal(L, "setSpotLightSpecular");

  lua_pushcclosure(L, lua_setSpotLightOptions, 1);
  lua_setglobal(L, "setSpotLightOptions");

  lua_pushcclosure(L, lua_setSpotLightCutoff, 1);
  lua_setglobal(L, "setSpotLightCutoff");

  lua_pushcclosure(L, lua_setSpotLightOn, 1);
  lua_setglobal(L, "setSpotLightTurnedOn");

  lua_pushcclosure(L, lua_isKeyPressed, 1);
  lua_setglobal(L, "isKeyPressed");
}
