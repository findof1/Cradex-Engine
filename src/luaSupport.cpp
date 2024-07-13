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

  lua_pushcclosure(L, lua_setGameObjectPosition, 1);
  lua_setglobal(L, "setGameObjectPosition");

  lua_pushcclosure(L, lua_setGameObjectRotation, 1);
  lua_setglobal(L, "setGameObjectRotation");

  lua_pushcclosure(L, lua_setGameObjectScale, 1);
  lua_setglobal(L, "setGameObjectScale");

  lua_pushcclosure(L, lua_addModel, 1);
  lua_setglobal(L, "addModel");

  lua_pushcclosure(L, lua_setModelPosition, 1);
  lua_setglobal(L, "setModelPosition");

  lua_pushcclosure(L, lua_setModelRotation, 1);
  lua_setglobal(L, "setModelRotation");

  lua_pushcclosure(L, lua_setModelScale, 1);
  lua_setglobal(L, "setModelScale");

  lua_pushcclosure(L, lua_addPointLight, 1);
  lua_setglobal(L, "addPointLight");
  /*
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
  */
  lua_pushcclosure(L, lua_addSpotLight, 1);
  lua_setglobal(L, "addSpotLight");

  /*
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
*/
}
