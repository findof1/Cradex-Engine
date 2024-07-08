#ifndef LUA_SUPPORT_H
#define LUA_SUPPORT_H

extern "C"
{
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include <string>
#include <iostream>
#include <renderer.hpp>

class LuaHandler
{
public:
  LuaHandler(Renderer *renderer);

  ~LuaHandler();

  bool loadScript(const std::string &scriptPath);

  bool executeFunction(const std::string &functionName);

  int getGlobalInt(const std::string &globalName);

  void setGlobalInt(const std::string &globalName, int value);

  void setGlobalNumber(const std::string &globalName, int value);

  void registerLuaFunctions();

private:
  Renderer *renderer;
  lua_State *L;
};

#endif