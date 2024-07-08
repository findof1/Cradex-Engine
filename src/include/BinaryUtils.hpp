#ifndef BINARY_SERIALIZER_H
#define BINARY_SERIALIZER_H

void binSerializeInt(std::ofstream &ofs, int value);
void binSerializeFloat(std::ofstream &ofs, float value);
void binSerializeString(std::ofstream &ofs, const std::string &str);
void binSerializeVector3(std::ofstream &ofs, const glm::vec3 &vec);
void binSerializeBool(std::ofstream &ofs, bool value);

void binUnserializeInt(std::ifstream &ifs, int &value);
void binUnserializeFloat(std::ifstream &ifs, float &value);
void binUnserializeString(std::ifstream &ifs, std::string &str);
void binUnserializeVector3(std::ifstream &ifs, glm::vec3 &vec);
void binUnserializeBool(std::ifstream &ifs, bool &value);

#endif