#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <shader.hpp>
#include <string>
#include <stb_image.h>
#include <vector>
#include <json.hpp>
using json = nlohmann::json;
class GameObject
{
public:
  int type;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  GameObject(int type, const char *diffuse, const char *specular, Shader shader);

  ~GameObject();

  void setDiffuse(const char *newDiffusePath);

  void setSpecular(const char *newSpecularPath);

  void draw(Shader shader);

  json serialize();

private:
  unsigned int diffuseMap;
  unsigned int specularMap;
  std::string diffusePath;
  std::string specularPath;
  unsigned int VBO;
  unsigned int VAO;
  int vertexCount;
  int indicesCount;

  unsigned int loadTexture(const char *path);

  void initCube();
};
#endif