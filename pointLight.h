#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

class PointLight
{
public:
  int id;
  PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float intensity, Shader shader, int id) : position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic), intensity(intensity), id(id)
  {

    shader.use();
    shader.setVec3("pointLights[" + std::to_string(id) + "].position", position.x, position.y, position.z);
    shader.setVec3("pointLights[" + std::to_string(id) + "].ambient", ambient.x, ambient.y, ambient.z);
    shader.setVec3("pointLights[" + std::to_string(id) + "].diffuse", diffuse.x, diffuse.y, diffuse.z);
    shader.setVec3("pointLights[" + std::to_string(id) + "].specular", specular.x, specular.y, specular.z);
    shader.setFloat("pointLights[" + std::to_string(id) + "].constant", constant);
    shader.setFloat("pointLights[" + std::to_string(id) + "].linear", linear);
    shader.setFloat("pointLights[" + std::to_string(id) + "].quadratic", quadratic);
    shader.setFloat("pointLights[" + std::to_string(id) + "].intensity", intensity);

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
    shader.setVec3("pointLights[" + std::to_string(id) + "].position", position.x, position.y, position.z);
  }

  void setAmbient(glm::vec3 newAmbient, Shader shader)
  {
    shader.use();
    ambient = newAmbient;
    shader.setVec3("pointLights[" + std::to_string(id) + "].ambient", ambient.x, ambient.y, ambient.z);
  }

  void setDiffuse(glm::vec3 newDiffuse, Shader shader)
  {
    shader.use();
    diffuse = newDiffuse;
    shader.setVec3("pointLights[" + std::to_string(id) + "].diffuse", diffuse.x, diffuse.y, diffuse.z);
  }

  void setSpecular(glm::vec3 newSpecular, Shader shader)
  {
    shader.use();
    specular = newSpecular;
    shader.setVec3("pointLights[" + std::to_string(id) + "].specular", specular.x, specular.y, specular.z);
  }

  void setIntensity(float newIntensity, Shader shader)
  {
    shader.use();
    intensity = newIntensity;
    shader.setFloat("pointLights[" + std::to_string(id) + "].intensity", intensity);
  }

  void setAttenuation(float newConstant, float newLinear, float newQuadratic, Shader shader)
  {
    shader.use();
    constant = newConstant;
    linear = newLinear;
    quadratic = newQuadratic;
    shader.setFloat("pointLights[" + std::to_string(id) + "].constant", constant);
    shader.setFloat("pointLights[" + std::to_string(id) + "].linear", linear);
    shader.setFloat("pointLights[" + std::to_string(id) + "].quadratic", quadratic);
  }

  void draw(Shader lightShader)
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

private:
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float constant;
  float linear;
  float quadratic;
  float intensity;
  unsigned int lightCubeVAO;
  unsigned int VBO;
};
#endif