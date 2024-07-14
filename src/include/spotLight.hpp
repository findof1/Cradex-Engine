#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <shader.hpp>
#include <json.hpp>
#include <string>
#include <vector>
using json = nlohmann::json;
class SpotLight
{
public:
  int id;
  glm::vec3 position;
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float constant;
  float linear;
  float quadratic;
  float cutOff;
  float outerCutOff;
  bool on;

  SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, Shader shader, int id);

  void setPosition(glm::vec3 newPosition, Shader shader);

  void setDirection(glm::vec3 newDirection, Shader shader);

  void setAmbient(glm::vec3 newAmbient, Shader shader);

  void setDiffuse(glm::vec3 newDiffuse, Shader shader);

  inline void setOnOrOff(bool value)
  {
    on = value;
  }

  void setSpecular(glm::vec3 newSpecular, Shader shader);

  void setCutOff(float newCutOff, float newOuterCutOff, Shader shader);

  void setAttenuation(float newConstant, float newLinear, float newQuadratic, Shader shader);

  void draw(Shader lightShader);

  json serialize();
  void binSerialize(std::ofstream &ofs);

private:
  unsigned int lightCubeVAO;
  unsigned int VBO;
};
#endif