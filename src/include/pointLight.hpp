#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <shader.hpp>
#include <json.hpp>
#include <string>
#include <vector>
using json = nlohmann::json;
class PointLight
{
public:
  int id;
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float constant;
  float linear;
  float quadratic;
  float intensity;

  PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float intensity, Shader shader, int id);

  void setPosition(glm::vec3 newPosition, Shader shader);

  void setAmbient(glm::vec3 newAmbient, Shader shader);

  void setDiffuse(glm::vec3 newDiffuse, Shader shader);

  void setSpecular(glm::vec3 newSpecular, Shader shader);

  void setIntensity(float newIntensity, Shader shader);

  void setAttenuation(float newConstant, float newLinear, float newQuadratic, Shader shader);

  void draw(Shader lightShader);

  json serialize();
  void binSerialize(std::ofstream &ofs);

private:
  unsigned int lightCubeVAO;
  unsigned int VBO;
};
#endif