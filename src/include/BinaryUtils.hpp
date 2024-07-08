#ifndef BINARY_SERIALIZER_H
#define BINARY_SERIALIZER_H

void binSerializeInt(std::ofstream &ofs, int value);

void binSerializeFloat(std::ofstream &ofs, float value);

void binSerializeString(std::ofstream &ofs, const std::string &str);

void binSerializeVector3(std::ofstream &ofs, const glm::vec3 &vec);

#endif