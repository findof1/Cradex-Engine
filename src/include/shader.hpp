#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Shader
{
public:
  unsigned int ID;

  Shader(const char *vertexPath, const char *fragmentPath);

  inline void use()
  {
    glUseProgram(ID);
  }

  inline void setBool(const std::string &name, bool value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }

  inline void setInt(const std::string &name, int value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  inline void setFloat(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

  inline void setVec3(const std::string &name, const float val1, const float val2, const float val3) const
  {
    float vecValues[] = {val1, val2, val3};
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, vecValues);
  }

  inline void setVec3(const std::string &name, glm::vec3 values) const
  {
    float vecValues[] = {values.x, values.y, values.z};
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, vecValues);
  }

  inline void setMat4(const std::string &name, const glm::mat4 &value) const
  {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
  }
};
#endif