#ifndef TEXTURE_UTIL_H
#define TEXTURE_UTIL_H

#include <string>
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

unsigned int GetTextureFromFile(const char *path, const std::string &directory);

#endif