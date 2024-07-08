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

void binSerializeBool(std::ofstream &ofs, bool value)
{
  char boolChar = value ? 1 : 0;
  ofs.write(reinterpret_cast<const char *>(&boolChar), sizeof(boolChar));
}

void binUnserializeInt(std::ifstream &ifs, int &value)
{
  ifs.read(reinterpret_cast<char *>(&value), sizeof(value));
}

void binUnserializeFloat(std::ifstream &ifs, float &value)
{
  ifs.read(reinterpret_cast<char *>(&value), sizeof(value));
}

void binUnserializeString(std::ifstream &ifs, std::string &str)
{
  size_t length;
  ifs.read(reinterpret_cast<char *>(&length), sizeof(length));
  str.resize(length);
  ifs.read(&str[0], length);
}

void binUnserializeVector3(std::ifstream &ifs, glm::vec3 &vec)
{
  binUnserializeFloat(ifs, vec.x);
  binUnserializeFloat(ifs, vec.y);
  binUnserializeFloat(ifs, vec.z);
}

void binUnserializeBool(std::ifstream &ifs, bool &value)
{
  char boolChar;
  ifs.read(reinterpret_cast<char *>(&boolChar), sizeof(boolChar));
  value = (boolChar != 0);
}