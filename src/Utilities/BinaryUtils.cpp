#include <BinaryUtils.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

void binSerializeInt(std::ofstream &ofs, int value)
{
  ofs.write(reinterpret_cast<const char *>(&value), sizeof(value));
}

void binSerializeFloat(std::ofstream &ofs, float value)
{
  ofs.write(reinterpret_cast<const char *>(&value), sizeof(value));
}

void binSerializeString(std::ofstream &ofs, const std::string &str)
{
  size_t length = str.size();
  ofs.write(reinterpret_cast<const char *>(&length), sizeof(length));
  ofs.write(str.data(), length);
}

void binSerializeVector3(std::ofstream &ofs, const glm::vec3 &vec)
{
  binSerializeFloat(ofs, vec.x);
  binSerializeFloat(ofs, vec.y);
  binSerializeFloat(ofs, vec.z);
}