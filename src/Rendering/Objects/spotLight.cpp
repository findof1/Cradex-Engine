#include <spotLight.hpp>
#include <BinaryUtils.hpp>

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff, Shader shader, int id) : position(position), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic), cutOff(cutOff), outerCutOff(outerCutOff), id(id)
{
  on = true;

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

void SpotLight::setPosition(glm::vec3 newPosition, Shader shader)
{
  shader.use();
  position = newPosition;
  shader.setVec3("spotLights[" + std::to_string(id) + "].position", position.x, position.y, position.z);
}

void SpotLight::setDirection(glm::vec3 newDirection, Shader shader)
{
  shader.use();
  direction = newDirection;
  shader.setVec3("spotLights[" + std::to_string(id) + "].direction", direction.x, direction.y, direction.z);
}

void SpotLight::setAmbient(glm::vec3 newAmbient, Shader shader)
{
  shader.use();
  ambient = newAmbient;
  shader.setVec3("spotLights[" + std::to_string(id) + "].ambient", ambient.x, ambient.y, ambient.z);
}

void SpotLight::setDiffuse(glm::vec3 newDiffuse, Shader shader)
{
  shader.use();
  diffuse = newDiffuse;
  shader.setVec3("spotLights[" + std::to_string(id) + "].diffuse", diffuse.x, diffuse.y, diffuse.z);
}

void SpotLight::setSpecular(glm::vec3 newSpecular, Shader shader)
{
  shader.use();
  specular = newSpecular;
  shader.setVec3("spotLights[" + std::to_string(id) + "].specular", specular.x, specular.y, specular.z);
}

void SpotLight::setIntensity(float newCutOff, float newOuterCutOff, Shader shader)
{
  shader.use();
  cutOff = newCutOff;
  outerCutOff = newOuterCutOff;
  shader.setFloat("spotLights[" + std::to_string(id) + "].cutOff", glm::cos(glm::radians(cutOff)));
  shader.setFloat("spotLights[" + std::to_string(id) + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));
}

void SpotLight::setAttenuation(float newConstant, float newLinear, float newQuadratic, Shader shader)
{
  shader.use();
  constant = newConstant;
  linear = newLinear;
  quadratic = newQuadratic;
  shader.setFloat("spotLights[" + std::to_string(id) + "].constant", constant);
  shader.setFloat("spotLights[" + std::to_string(id) + "].linear", linear);
  shader.setFloat("spotLights[" + std::to_string(id) + "].quadratic", quadratic);
}

void SpotLight::draw(Shader lightShader)
{
  if (on)
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

json SpotLight::serialize()
{
  json serializedSpotLight;

  json serializedPosition;
  serializedPosition["x"] = position.x;
  serializedPosition["y"] = position.y;
  serializedPosition["z"] = position.z;

  serializedSpotLight["position"] = serializedPosition;

  json serializedDirection;
  serializedDirection["x"] = direction.x;
  serializedDirection["y"] = direction.y;
  serializedDirection["z"] = direction.z;

  serializedSpotLight["direction"] = serializedDirection;

  json serializedAmbient;
  serializedAmbient["r"] = ambient.x;
  serializedAmbient["g"] = ambient.y;
  serializedAmbient["b"] = ambient.z;

  serializedSpotLight["ambient"] = serializedAmbient;

  json serializedDiffuse;
  serializedDiffuse["r"] = diffuse.x;
  serializedDiffuse["g"] = diffuse.y;
  serializedDiffuse["b"] = diffuse.z;

  serializedSpotLight["diffuse"] = serializedDiffuse;

  json serializedSpecular;
  serializedSpecular["r"] = specular.x;
  serializedSpecular["g"] = specular.y;
  serializedSpecular["b"] = specular.z;

  serializedSpotLight["specular"] = serializedSpecular;

  serializedSpotLight["constant"] = constant;

  serializedSpotLight["linear"] = linear;

  serializedSpotLight["quadratic"] = quadratic;

  serializedSpotLight["cutOff"] = cutOff;

  serializedSpotLight["outerCutOff"] = outerCutOff;

  serializedSpotLight["on"] = on;

  return serializedSpotLight;
}

void SpotLight::binSerialize(std::ofstream &ofs)
{
  binSerializeVector3(ofs, position);
  binSerializeVector3(ofs, direction);
  binSerializeVector3(ofs, ambient);
  binSerializeVector3(ofs, diffuse);
  binSerializeVector3(ofs, specular);
  binSerializeFloat(ofs, constant);
  binSerializeFloat(ofs, linear);
  binSerializeFloat(ofs, quadratic);
  binSerializeFloat(ofs, cutOff);
  binSerializeFloat(ofs, outerCutOff);
  binSerializeInt(ofs, on);
}
