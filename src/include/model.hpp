#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <mesh.hpp>
#include <json.hpp>

using json = nlohmann::json;
class Model
{
public:
  std::vector<Texture> textures_loaded;
  std::vector<Mesh> meshes;
  std::string directory;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  std::string path;

  Model(const std::string &path, glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f));

  void draw(Shader &shader);

  json serialize();

private:
  void loadModel(std::string path);

  void processNode(aiNode *node, const aiScene *scene);

  Mesh processMesh(aiMesh *mesh, const aiScene *scene);

  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif