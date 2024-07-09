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
  std::size_t length;
  ifs.read(reinterpret_cast<char *>(&length), sizeof(length));

  if (ifs.fail())
  {
    std::cerr << "Failed to read string length" << std::endl;
    throw std::runtime_error("Failed to read string length");
  }

  if (length > 1000000)
  {
    std::cerr << "String length too large: " << length << std::endl;
    throw std::bad_alloc();
  }

  str.resize(length);
  ifs.read(&str[0], length);

  if (ifs.fail())
  {
    std::cerr << "Failed to read string data" << std::endl;
    throw std::runtime_error("Failed to read string data");
  }
}

void binUnserializeVector3(std::ifstream &ifs, glm::vec3 &vec)
{
  binUnserializeFloat(ifs, vec.x);
  binUnserializeFloat(ifs, vec.y);
  binUnserializeFloat(ifs, vec.z);

  if (ifs.fail())
  {
    std::cerr << "Failed to read vec3 data" << std::endl;
    throw std::runtime_error("Failed to read vec3 data");
  }
}

void binUnserializeBool(std::ifstream &ifs, bool &value)
{
  char boolChar;
  ifs.read(reinterpret_cast<char *>(&boolChar), sizeof(boolChar));
  value = (boolChar != 0);
}