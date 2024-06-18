#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

class SpotLight
{
public:
  int id;
  SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, Shader shader, int id) : position(position), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic), cutOff(cutOff), outerCutOff(outerCutOff), id(id)
  {
    show = true;

    shader.use();
    shader.setVec3("spotLights[" + std::to_string(id) + "].position", position.x, position.y, position.z);
    shader.setVec3("spotLights[" + std::to_string(id) + "].direction", direction.x, direction.y, direction.z);
    shader.setVec3("spotLights[" + std::to_string(id) + "].ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3("spotLights[" + std::to_string(id) + "].diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader.setVec3("spotLights[" + std::to_string(id) + "].specular", specular.x, specular.y, specular.z);
    shader.setFloat("spotLights[" + std::to_string(id) + "].constant", constant);
    shader.setFloat("spotLights[" + std::to_string(id) + "].linear", linear);
    shader.setFloat("spotLights[" + std::to_string(id) + "].quadratic", quadratic);
    shader.setFloat("spotLights[" + std::to_string(id) + "].cutOff", glm::cos(glm::radians(cutOff)));
    shader.setFloat("spotLights[" + std::to_string(id) + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
  }

  void setPosition(glm::vec3 newPosition, Shader shader)
  {
    shader.use();
    position = newPosition;
    shader.setVec3("spotLights[" + std::to_string(id) + "].position", position.x, position.y, position.z);
  }

  void setDirection(glm::vec3 newDirection, Shader shader)
  {
    shader.use();
    direction = newDirection;
    shader.setVec3("spotLights[" + std::to_string(id) + "].direction", direction.x, direction.y, direction.z);
  }

  void setAmbient(glm::vec3 newAmbient, Shader shader)
  {
    shader.use();
    ambient = newAmbient;
    shader.setVec3("spotLights[" + std::to_string(id) + "].ambient", ambient.x, ambient.y, ambient.z);
  }

  void setDiffuse(glm::vec3 newDiffuse, Shader shader)
  {
    shader.use();
    diffuse = newDiffuse;
    shader.setVec3("spotLights[" + std::to_string(id) + "].diffuse", diffuse.x, diffuse.y, diffuse.z);
  }

  void setVisible(bool value)
  {
    show = value;
  }

  void setSpecular(glm::vec3 newSpecular, Shader shader)
  {
    shader.use();
    specular = newSpecular;
    shader.setVec3("spotLights[" + std::to_string(id) + "].specular", specular.x, specular.y, specular.z);
  }

  void setIntensity(float newCutOff, float newOuterCutOff, Shader shader)
  {
    shader.use();
    cutOff = newCutOff;
    outerCutOff = newOuterCutOff;
    shader.setFloat("spotLights[" + std::to_string(id) + "].cutOff", glm::cos(glm::radians(cutOff)));
    shader.setFloat("spotLights[" + std::to_string(id) + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));
  }

  void setAttenuation(float newConstant, float newLinear, float newQuadratic, Shader shader)
  {
    shader.use();
    constant = newConstant;
    linear = newLinear;
    quadratic = newQuadratic;
    shader.setFloat("spotLights[" + std::to_string(id) + "].constant", constant);
    shader.setFloat("spotLights[" + std::to_string(id) + "].linear", linear);
    shader.setFloat("spotLights[" + std::to_string(id) + "].quadratic", quadratic);
  }

  void draw(Shader lightShader)
  {
    if (show)
    {
      lightShader.use();
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, position);
      model = glm::scale(model, glm::vec3(0.2f));
      lightShader.setMat4("model", model);
      lightShader.setVec3("color", diffuse.x, diffuse.y, diffuse.z);
      glBindVertexArray(lightCubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
    }
  }

private:
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
  bool show;
  unsigned int lightCubeVAO;
  unsigned int VBO;
};
#endif